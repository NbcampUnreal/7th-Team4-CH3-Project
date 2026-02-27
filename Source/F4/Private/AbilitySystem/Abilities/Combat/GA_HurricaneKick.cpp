#include "AbilitySystem/Abilities/Combat/GA_HurricaneKick.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/Base/F4CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/F4GameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"

UGA_HurricaneKick::UGA_HurricaneKick()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; 
	
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Combat_HurricaneKick)); 
	ActivationOwnedTags.AddTag(F4GameplayTags::Character_State_HurricaneKicking);
}

void UGA_HurricaneKick::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
	)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AF4CharacterBase* Character = Cast<AF4CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return; 
	}
	
	if (CostEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CostEffect);
		CostEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
	
	if (SpeedEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(SpeedEffect);
		SpeedEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
	
	ToggleTimers(true);
	
	// Wait Stamina Exhaust Task 
	UAbilityTask_WaitGameplayEvent* WaitStaminaExhaustTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		F4GameplayTags::Event_Stamina_Exhuast
	);
	
	if (WaitStaminaExhaustTask)
	{
		WaitStaminaExhaustTask->EventReceived.AddDynamic(this,&ThisClass::OnStaminaExhausted);
		WaitStaminaExhaustTask->ReadyForActivation();
	}
	
	// Montage Task 
	if (!HurricaneKickMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);    
		return; 
	}
	
	StopSectionName = HurricaneKickMontage->GetSectionName(HurricaneKickMontage->GetNumSections()-1); 
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = 
	 	UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	 		this,
	 		TEXT("HurricaneKickTask"),
	 		HurricaneKickMontage
	 	);
    
	if (PlayMontageTask)
	{
		PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnCompleted);
		PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCompleted);
		PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted); 
		PlayMontageTask->ReadyForActivation();
	}
	
	// Gameplay Cue 
	Character->GetAbilitySystemComponent()->AddGameplayCue(F4GameplayTags::GameplayCue_HurricaneKick); 
	Character->GetCharacterMovement()->bOrientRotationToMovement = true; 
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true);
}

void UGA_HurricaneKick::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ToggleTimers(false); 
	
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (SpeedEffectHandle.IsValid())
		{
			BP_RemoveGameplayEffectFromOwnerWithHandle(SpeedEffectHandle);
		}
		
		if (CostEffectHandle.IsValid())
		{
			BP_RemoveGameplayEffectFromOwnerWithHandle(CostEffectHandle);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HurricaneKick::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	StopHurricaneKick(); 
}

void UGA_HurricaneKick::ToggleTimers(bool bStart)
{
	if (bStart)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DurationTimerHandle, 
			this, 
			&UGA_HurricaneKick::StopHurricaneKick,
			ActiveDuration,
			false
		);
	
		GetWorld()->GetTimerManager().SetTimer(
			HitTimerHandle,
			this,
			&ThisClass::DetectEnemies,
			0.2,
			true
		);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HitTimerHandle);
	}
}

void UGA_HurricaneKick::DetectEnemies()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character) return;
	
	const FVector StartLocation = Character->GetMesh()->GetBoneLocation(TraceStartBoneName);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	
	TArray<FHitResult> OutHits; 
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		Character->GetWorld(),
		StartLocation,
		StartLocation,
		TraceRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		OutHits,
		false
	);
	
	if (bHit && OutHits.Num() != 0)
	{
		for (int32 i = 0; i < OutHits.Num(); i++)
		{
			AActor* HitActor = OutHits[i].GetActor();
			if (HitActor) continue;
			
			FGameplayEventData Payload;
			Payload.Instigator = GetAvatarActorFromActorInfo(); 
			Payload.Target = HitActor;
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, F4GameplayTags::Event_Hit_Damage, Payload);
		}
	}
}

void UGA_HurricaneKick::OnCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false); 
}

void UGA_HurricaneKick::OnStaminaExhausted(FGameplayEventData Data)
{
	StopHurricaneKick(); 
}

void UGA_HurricaneKick::StopHurricaneKick()
{
	AF4CharacterBase* Character = Cast<AF4CharacterBase>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_JumpToSection(StopSectionName, HurricaneKickMontage);
		}
		
		Character->GetAbilitySystemComponent()->RemoveGameplayCue(F4GameplayTags::GameplayCue_HurricaneKick);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
