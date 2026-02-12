#include "AbilitySystem/Abilities/Movement/GA_Sprint.h"
#include "System/F4GameplayTags.h"

UGA_Sprint::UGA_Sprint()
{
	FGameplayTagContainer DefaultTagContainer;
	DefaultTagContainer.AddTag(F4GameplayTags::Ability_Movement_Sprint);
	SetAssetTags(DefaultTagContainer);
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
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

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (SprintEffectHandle.IsValid())
	{
		BP_RemoveGameplayEffectFromOwnerWithHandle(SprintEffectHandle);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
