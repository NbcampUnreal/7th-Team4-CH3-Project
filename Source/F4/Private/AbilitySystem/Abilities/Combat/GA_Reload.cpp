#include "AbilitySystem/Abilities/Combat/GA_Reload.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Attributes/F4AttributeSetWeapon.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Firearm.h"
#include "Inventory/F4ItemInstance.h"
#include "System/F4GameplayTags.h"

UGA_Reload::UGA_Reload()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Combat_Reload));

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Reloading);
	ActivationBlockedTags.AddTag(F4GameplayTags::Character_State_Sprinting);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);
	ActivationBlockedTags.AddTag(F4GameplayTags::Character_State_Rolling);

	ActivationOwnedTags.AddTag(F4GameplayTags::State_Reloading);

	// TODO: cost - ammo(총알) 인벤토리에 있어야 장전가능 일단은 그냥 총알 무한으로 장전가능하게
	// TODO: 이미 탄창이 MAX라면 실행안되게
}

bool UGA_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AActor* Avatar = ActorInfo->AvatarActor.Get();
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!Avatar || !ASC)
	{
		return false;
	}

	float CurrentAmmo = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetCurrentAmmoAttribute());
	float MaxAmmo = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetMaxAmmoAttribute());
	if (CurrentAmmo >= MaxAmmo)
	{
		return false;
	}

	UF4EquipmentComponent* EquipComp = Avatar->FindComponentByClass<UF4EquipmentComponent>();
	UF4InventoryComponent* InvenComp = Avatar->FindComponentByClass<UF4InventoryComponent>();
	if (!EquipComp || !InvenComp)
	{
		return false;
	}

	UF4ItemInstance* ActiveWeapon = EquipComp->GetActiveWeaponInstance();
	if (!ActiveWeapon)
	{
		return false;
	}

	const UF4ItemFragment_Firearm* FirearmFrag = ActiveWeapon->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Firearm>();
	if (!FirearmFrag || InvenComp->GetTotalItemCountByDefinition(FirearmFrag->AmmoItemDefinition) <= 0)
	{
		return false;
	}

	return true;
}

void UGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ReloadMontage, 1.0f);
	if (!PlayMontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Reload::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Reload::OnMontageCancelled);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Reload::OnMontageCancelled);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, F4GameplayTags::Event_Montage_Reload);
	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_Reload::OnReloadGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Reload::OnMontageCompleted()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Reload::OnMontageCancelled()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Reload::OnReloadGameplayEvent(FGameplayEventData EventData)
{
	PerformReload();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Reload::PerformReload()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!ASC || !Avatar || !ReloadEffectClass)
	{
		return;
	}

    UF4InventoryComponent* InvenComp = Avatar->FindComponentByClass<UF4InventoryComponent>();
    UF4EquipmentComponent* EquipComp = Avatar->FindComponentByClass<UF4EquipmentComponent>();
	if (!InvenComp || !EquipComp)
	{
		return;
	}

    UF4ItemInstance* WeaponInstance = EquipComp->GetActiveWeaponInstance();
	if (!WeaponInstance)
	{
		return;
	}

    const UF4ItemFragment_Firearm* FirearmFrag = WeaponInstance->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Firearm>();
	if (!FirearmFrag || !FirearmFrag->AmmoItemDefinition)
	{
		return;
	}

    float CurrentAmmo = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetCurrentAmmoAttribute());
    float MaxAmmo = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetMaxAmmoAttribute());
    int32 SpaceLeft = FMath::FloorToInt(MaxAmmo - CurrentAmmo);

	if (SpaceLeft <= 0)
	{
		return;
	}

    int32 AmmoInInventory = InvenComp->GetTotalItemCountByDefinition(FirearmFrag->AmmoItemDefinition);
    int32 AmountToReload = FMath::Min(SpaceLeft, AmmoInInventory);

	if (AmountToReload <= 0)
	{
		return;
	}

    InvenComp->ConsumeItemByDefinition(FirearmFrag->AmmoItemDefinition, AmountToReload);

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    ContextHandle.AddSourceObject(Avatar);

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ReloadEffectClass, GetAbilityLevel(), ContextHandle);

    if (SpecHandle.IsValid())
    {
        SpecHandle.Data.Get()->SetSetByCallerMagnitude(F4GameplayTags::Data_Reload_Ammo, static_cast<float>(AmountToReload));
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
}
