#include "AbilitySystem/Abilities/Movement/GA_Sprint.h"

#include "AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"
#include "GameplayEffect.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"
#include "Abilities/Tasks/AbilityTask_WaitVelocityChange.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"

UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	AbilityTags.AddTag(F4GameplayTags::Ability_Movement_Sprint);
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_Sprinting);
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_NoRegenStamina);
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
	UAbilityTask_WaitAttributeChange* WaitAttributeChangeTask = UAbilityTask_WaitAttributeChange::WaitForAttributeChange(
		this,
		UF4AttributeSetCharacter::GetStaminaAttribute(),
		FGameplayTag::EmptyTag,
		FGameplayTag::EmptyTag,
		false
	);
	
	if (WaitAttributeChangeTask)
	{
		WaitAttributeChangeTask->OnChange.AddDynamic(this,&ThisClass::OnStaminaChanged);
		WaitAttributeChangeTask->ReadyForActivation();
	}
	
	
	// Check Released 
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	if (WaitInputReleaseTask)
	{
		WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputReleased);
		WaitInputReleaseTask->ReadyForActivation(); 
	}
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

void UGA_Sprint::OnInputReleased(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false); 
}

void UGA_Sprint::OnStaminaChanged()
{
	float CurrentStamina = 
		GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UF4AttributeSetCharacter::GetStaminaAttribute());

	if (CurrentStamina <= 0.01f)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
