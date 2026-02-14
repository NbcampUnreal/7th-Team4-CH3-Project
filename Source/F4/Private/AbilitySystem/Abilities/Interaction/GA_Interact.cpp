#include "AbilitySystem/Abilities/Interaction/GA_Interact.h"

#include "Interface/Interactable.h"
#include "System/F4GameplayTags.h"

UGA_Interact::UGA_Interact()
{
	AbilityTags.AddTag(F4GameplayTags::Ability_Interaction_Interact);
	
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_Interacting); 
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
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AvatarPawn);
	
	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults, 
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(InteractionRadius),
		Params
	);
	
	if (bDrawTrace)
	{
		// Debug 
		DrawDebugSphere(
			GetWorld(),
			StartLocation,
			InteractionRadius,
			InteractionDistance,
			TraceColor,
			false,
			DrawTime
		);
		
		DrawDebugSphere(
		GetWorld(),
		EndLocation,
		InteractionRadius,
		InteractionDistance,
		TraceColor,
		false,
		DrawTime
	);
		
	}
	
	if (bHit)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor == nullptr) continue;
			
			IInteractable* Interactable = Cast<IInteractable>(HitActor);
			if (Interactable)
			{
				Interactable->DoInteract(AvatarPawn);
				break; 
			}
		}
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false); 
}
