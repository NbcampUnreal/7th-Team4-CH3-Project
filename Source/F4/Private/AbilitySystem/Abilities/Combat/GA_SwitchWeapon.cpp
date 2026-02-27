#include "AbilitySystem/Abilities/Combat/GA_SwitchWeapon.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Inventory/F4EquipmentComponent.h"
#include "System/F4GameplayTags.h"

UGA_SwitchWeapon::UGA_SwitchWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = F4GameplayTags::Event_Weapon_Switch;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_Reload);
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Switching_Weapon);
}

void UGA_SwitchWeapon::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!TriggerEventData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CachedTargetSlotIndex = static_cast<int32>(TriggerEventData->EventMagnitude);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		SwitchMontage,
		1.0f
	);

	if (!PlayMontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_SwitchWeapon::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_SwitchWeapon::OnMontageCancelled);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_SwitchWeapon::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		F4GameplayTags::Event_Montage_Switching
	);

	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_SwitchWeapon::OnSwitchingGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_SwitchWeapon::OnMontageCompleted()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_SwitchWeapon::OnMontageCancelled()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_SwitchWeapon::OnSwitchingGameplayEvent(FGameplayEventData EventData)
{
	PerformSwitch();
}

void UGA_SwitchWeapon::PerformSwitch()
{
	AF4PlayerCharacter* Character = Cast<AF4PlayerCharacter>(GetAvatarActorFromActorInfo());
	UF4EquipmentComponent* EquipmentComp = Character->FindComponentByClass<UF4EquipmentComponent>();
	if (!Character || !EquipmentComp)
	{
		return;
	}

	EWeaponSlot TargetSlot = static_cast<EWeaponSlot>(CachedTargetSlotIndex);
	EquipmentComp->SetActiveWeapon(TargetSlot);
}

