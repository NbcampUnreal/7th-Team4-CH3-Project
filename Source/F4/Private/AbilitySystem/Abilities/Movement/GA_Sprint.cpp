#include "AbilitySystem/Abilities/Movement/GA_Sprint.h"

#include "AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"
#include "GameplayEffect.h"

UGA_Sprint::UGA_Sprint()
{
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Movement_Sprint));
	
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_Sprinting);
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (CostGameplayEffectClass)
	{
		CostEffectHandle = ApplyGameplayEffectToOwner(
			Handle, 
			ActorInfo,
			ActivationInfo,
			CostGameplayEffectClass.GetDefaultObject(),
			GetAbilityLevel()
		);
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	if (SprintEffect)
	{
		SprintEffectHandle = ApplyGameplayEffectToOwner(
			Handle, 
			ActorInfo,
			ActivationInfo, 
			SprintEffect
			.GetDefaultObject(), 
			GetAbilityLevel()
		);
	}
	
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 1. Remove Speed Effect 
	if (SprintEffectHandle.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			if (SprintEffectHandle.IsValid())
			{
				ASC->RemoveActiveGameplayEffect(SprintEffectHandle);
				SprintEffectHandle.Invalidate();
			}
			
			if (CostEffectHandle.IsValid())
			{
				ASC->RemoveActiveGameplayEffect(CostEffectHandle);
				CostEffectHandle.Invalidate();
			}
			
			
			UE_LOG(LogTemp, Warning, TEXT("Sprint Effect Removed via Handle!"));
		}
		
		
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
