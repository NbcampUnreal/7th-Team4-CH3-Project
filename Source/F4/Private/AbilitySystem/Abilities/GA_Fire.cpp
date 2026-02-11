#include "AbilitySystem/Abilities/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/F4GameplayTags.h"

UGA_Fire::UGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(F4GameplayTags::Ability_Combat_Fire);

	// TODO: 달리기 중이라면 끊기
	// CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Movement_Sprint)

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Firing);

	// TODO: cost bullet 추가 필요
}

void UGA_Fire::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		FireMontage,
		1.0f
	);

	if (!PlayMontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Fire::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Fire::OnMontageCancelled);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Fire::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		F4GameplayTags::Event_Montage_Fire
	);

	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_Fire::OnFireGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_Fire::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Fire::OnMontageCompleted()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Fire::OnMontageCancelled()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Fire::OnFireGameplayEvent(FGameplayEventData EventData)
{
	PerformFireTrace();
}

// TODO: trace 대신 무기 발사로직?
void UGA_Fire::PerformFireTrace()
{
	// TODO: 캐릭터 클래스 sync?
	ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!AvatarCharacter || !AvatarCharacter->GetMesh())
	{
		return;
	}

	// TODO: 소켓이름 sync
	FName RightHandSocketName = TEXT("hand_r");
	FVector Start = AvatarCharacter->GetMesh()->GetSocketLocation(RightHandSocketName);
	FVector Forward = AvatarCharacter->GetActorForwardVector();

	FVector End = Start + Forward * TraceDistance;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarCharacter);

	FHitResult HitResult;

	// TODO: Trace 수정 (TO 투사체 등)
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor)
		{
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor"));

		// TODO: 인터페이스 상속받은 적한테만 피격판정할꺼면 인터페이스 cast?
		UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>();
		if (!TargetASC)
		{
			return;
		}

		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		EffectContext.AddHitResult(HitResult);
		EffectContext.AddSourceObject(AvatarCharacter);

		FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			DamageEffectClass,
			1.0f,
			EffectContext
		);

		if (SpecHandle.IsValid())
		{
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				TargetASC
			);
		}
	}
}
