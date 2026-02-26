#include "AbilitySystem/Abilities/Itmes/GA_Potion_Invincible.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemInstance.h"

UGA_Potion_Invincible::UGA_Potion_Invincible()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Potion_Invincible::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (AvatarCharacter && ASC)
	{
		if (InvincibilityEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle =
				ASC->MakeOutgoingSpec(InvincibilityEffectClass, 1.0f, EffectContext);
			ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

		// 원래 Materials 백업
		if (USkeletalMeshComponent* Mesh = AvatarCharacter->GetMesh())
		{
			for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
			{
				OriginalMaterials.Add(Mesh->GetMaterial(i));
				if (TransparentMaterial)
				{
					Mesh->SetMaterial(i, TransparentMaterial);
				}
			}
		}
	}

	if (UF4ItemInstance* ItemInst = Cast<UF4ItemInstance>(GetCurrentSourceObject()))
	{
		if (UF4InventoryComponent* InvComp = AvatarCharacter->FindComponentByClass<UF4InventoryComponent>())
		{
			InvComp->ConsumeItem(ItemInst, 1);
		}
	}

	// 강제 해제 조건
	UAbilityTask_WaitGameplayTagAdded* WaitFireTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this, FGameplayTag::RequestGameplayTag(TEXT("Ability.Combat.Fire")));
	WaitFireTag->Added.AddDynamic(this, &UGA_Potion_Invincible::OnActionDetected);
	WaitFireTag->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* WaitSprintTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this, FGameplayTag::RequestGameplayTag(TEXT("Ability.Movement.Sprint")));
	WaitSprintTag->Added.AddDynamic(this, &UGA_Potion_Invincible::OnActionDetected);
	WaitSprintTag->ReadyForActivation();

	// 5초 경과
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, Duration);
	WaitDelay->OnFinish.AddDynamic(this, &UGA_Potion_Invincible::OnDurationEnded);
	WaitDelay->ReadyForActivation();
}

void UGA_Potion_Invincible::OnDurationEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Potion_Invincible::OnActionDetected()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Potion_Invincible::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       bool bReplicateEndAbility, bool bWasCancelled)
{
	ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (AvatarCharacter)
	{
		if (USkeletalMeshComponent* Mesh = AvatarCharacter->GetMesh())
		{
			for (int i = 0; i < OriginalMaterials.Num(); ++i)
			{
				if (OriginalMaterials[i])
				{
					Mesh->SetMaterial(i, OriginalMaterials[i]);
				}
			}
		}
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC && ActiveEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
		ActiveEffectHandle.Invalidate();
	}

	OriginalMaterials.Empty();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
