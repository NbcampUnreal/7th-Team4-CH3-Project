#include "AbilitySystem/Abilities/Combat/GA_Death.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Characters/Player/F4PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/BattleGameMode.h"
#include "System/F4GameplayTags.h"


UGA_Death::UGA_Death()
{
	AbilityTags.AddTag(F4GameplayTags::Ability_Death);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
	Character->GetCharacterMovement()->SetMovementMode(MOVE_None);

	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = 
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, 	
			NAME_None,
			DeathMontage,
			5.0f
		); 
	
	PlayMontageTask->ReadyForActivation();
	
	UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.4f); 
	WaitDelayTask->ReadyForActivation(); 
	
	WaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnEndDelay);
}


void UGA_Death::OnEndDelay()
{
	AF4PlayerCharacter* Character = Cast<AF4PlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// OpenLevel 이전에 포인터를 미리 캐싱
	ABattleGameMode* GM = nullptr;
	AF4PlayerController* PC = Cast<AF4PlayerController>(Character->GetController());

	if (UWorld* World = GetWorld())
	{
		GM = Cast<ABattleGameMode>(World->GetAuthGameMode());
	}

	// OpenLevel이 월드를 파괴하기 전에 GAS 정리를 먼저 완료
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	if (GM)
	{
		GM->HandlePlayerDeath(PC);
	}
}
