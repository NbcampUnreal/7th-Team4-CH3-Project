#include "AbilitySystem/Abilities/Movement/GA_Roll.h"
#include"Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "System/F4GameplayTags.h"

UGA_Roll::UGA_Roll()
{
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Movement_Roll));

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Switching_Weapon);

	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_Fire);
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_Reload);
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_HurricaneKick);
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_Aim);
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_Invisible);
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_SwitchWeapon);

	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_Rolling);
}

void UGA_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (RollMontage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	AActor* Actor = GetAvatarActorFromActorInfo(); 
	if (Actor == nullptr) return;
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = 
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, 	
			NAME_None,
			RollMontage,
			1.0f
		); 
	
	PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCancelled);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageCancelled);
	
	PlayMontageTask->ReadyForActivation();
}

void UGA_Roll::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Roll::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
