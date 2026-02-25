#include "AbilitySystem/Abilities/Enemy/GA_AttackRange.h"
#include"AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_AttackRange::UGA_AttackRange()
{
	
}

void UGA_AttackRange::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
	if (!CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true,false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask = 
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, 
		NAME_None,
		AttackMontage,
		1.0f
		);
	
	if (!MontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	MontageTask->OnCompleted.AddDynamic(this, &UGA_AttackRange::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_AttackRange::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_AttackRange::OnMontageCancelled);
	
	MontageTask->ReadyForActivation();
	
	/*UAbilityTask_WaitGameplayEvent* WaitEventTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			FGameplayTag::RequestGameplayTag(FName("Event.Montage.DealDamage"))
		);
	
	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_AttackRange::OnDamageGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}*/
}