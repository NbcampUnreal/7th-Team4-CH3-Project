#include "AbilitySystem/Abilities/Movement/GA_Sprint.h"

#include "AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"
#include "GameplayEffect.h"
#include "Abilities/Tasks/AbilityTask_WaitVelocityChange.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Movement_Sprint));
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_Sprinting);
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_NoRegenStamina);
}

bool UGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                    FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character) return false;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent) return false;

	if (MovementComponent->IsFalling()) return false;

	const FVector Velocity = MovementComponent->Velocity;

	const float CurrentSpeed = Velocity.Size2D();
	if (CurrentSpeed <= 30.f) return false;

	return true;
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (CostEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CostEffect);
		CostEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}

	if (SprintEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(SprintEffect);
		SprintEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}

	// Check Stamina 
	UAbilityTask_WaitAttributeChange* WaitAttributeChangeTask =
		UAbilityTask_WaitAttributeChange::WaitForAttributeChange(
			this,
			UF4AttributeSetCharacter::GetStaminaAttribute(),
			FGameplayTag::EmptyTag,
			FGameplayTag::EmptyTag,
			false
		);

	if (WaitAttributeChangeTask)
	{
		WaitAttributeChangeTask->OnChange.AddDynamic(this, &ThisClass::OnStaminaChanged);
		WaitAttributeChangeTask->ReadyForActivation();
	}
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                            bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (SprintEffectHandle.IsValid())
		{
			BP_RemoveGameplayEffectFromOwnerWithHandle(SprintEffectHandle);;
		}

		if (CostEffectHandle.IsValid())
		{
			BP_RemoveGameplayEffectFromOwnerWithHandle(CostEffectHandle);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UGA_Sprint::OnStaminaChanged()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character->GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero() || Character->GetCharacterMovement()
		->IsFalling())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	float CurrentStamina =
		GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UF4AttributeSetCharacter::GetStaminaAttribute());

	if (CurrentStamina <= 0.01f)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
