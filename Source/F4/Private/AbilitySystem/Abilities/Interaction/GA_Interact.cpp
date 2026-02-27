#include "AbilitySystem/Abilities/Interaction/GA_Interact.h"

#include "Interface/Interactable.h"
#include "System/F4GameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UGA_Interact::UGA_Interact()
{
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Interaction_Interact));
	
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_Interacting); 
	
	ActivationBlockedTags.AddTag(F4GameplayTags::Character_State_Interacting);
}

void UGA_Interact::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
		return;
	}

	APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (!AvatarPawn) return;
	
	FVector StartLocation = AvatarPawn->GetPawnViewLocation();
	FVector EndLocation = StartLocation + (AvatarPawn->GetViewRotation().Vector() * InteractionDistance);
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AvatarPawn);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		StartLocation,
		EndLocation,
		ECC_Visibility,
		Params
	);
	
	if (bDrawTrace)
	{
		// Debug 
		DrawDebugLine(
			GetWorld(),
			StartLocation,
			EndLocation,
			TraceColor,
			false,
			DrawTime,
			0,
			15.0f
		);
	}
	
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			IInteractable* Interactable = Cast<IInteractable>(HitActor);
			if (Interactable)
			{
				Interactable->DoInteract(AvatarPawn);
			}
		}
	}
	
	UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, InteractionCoolDown);
	if (WaitDelayTask)
	{
		WaitDelayTask->OnFinish.AddDynamic(this, &UGA_Interact::OnInteractFinished);
		WaitDelayTask->ReadyForActivation();
	}
}

void UGA_Interact::OnInteractFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,true, false);
}
