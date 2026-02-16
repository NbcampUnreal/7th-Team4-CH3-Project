#include "AbilitySystem/Abilities/Movement/GA_Jump.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "System/F4GameplayTags.h"
#include"Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"

UGA_Jump::UGA_Jump()
{
	AbilityTags.AddTag(F4GameplayTags::Ability_Movement_Jump); 
	
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_InAir);
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 1. Check Commit and Avatar Info
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true ,true);
		return; 
	}
	
	AActor* Actor = GetAvatarActorFromActorInfo();
	ACharacter* Character = Cast<ACharacter>(Actor);
	if (Character)
	{
		// 2. Jump
		Character->ACharacter::Jump(); 
		
		// 3. MovementMode Change 감지 Task 
		UAbilityTask_WaitMovementModeChange* WaitLandingTask = 
			UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, MOVE_Walking);
		
		WaitLandingTask->OnChange.AddDynamic(this, &UGA_Jump::OnCharacterLanded);
		
		WaitLandingTask->ReadyForActivation(); 
	}
}

void UGA_Jump::OnCharacterLanded(EMovementMode NewMovementMode)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
