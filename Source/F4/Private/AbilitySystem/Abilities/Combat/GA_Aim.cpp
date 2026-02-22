#include "AbilitySystem/Abilities/Combat/GA_Aim.h"

#include "GameFramework/Character.h"
#include "System/F4GameplayTags.h"

UGA_Aim::UGA_Aim()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Combat_Aim));
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Aiming);

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Reloading);
}

void UGA_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// TODO: Ability 에서 할게 아닌 것 같음 (해당 값을 AttributeSet으로 빼서 GE로 처리하던가? 등등
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->bUseControllerRotationYaw = true;
	}
}

void UGA_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// TODO: Ability 에서 할게 아닌 것 같음 (해당 값을 AttributeSet으로 빼서 GE로 처리하던가? 등등
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->bUseControllerRotationYaw = false;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Aim::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (IsActive())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

