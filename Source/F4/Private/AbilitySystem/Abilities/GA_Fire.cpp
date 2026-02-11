#include "AbilitySystem/Abilities/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/Character.h"
#include "System/F4GameplayTags.h"

UGA_Fire::UGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(F4GameplayTags::Ability_Combat_Fire);

	// 달리기 중이라면 끊기
	// CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Movement_Sprint)

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Firing);
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

void UGA_Fire::PerformFireTrace()
{
	// TODO: 캐릭터 클래스 sync?
	ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!AvatarCharacter || AvatarCharacter->GetMesh())
	{
		return;
	}

	// TODO: 소켓이름 sync
	FName RightHandSocketName = TEXT("RightHand_01");
	FVector Start = AvatarCharacter->GetMesh()->GetSocketLocation(RightHandSocketName);
	FVector Forward = AvatarCharacter->GetActorForwardVector();

	FVector End = Start + Forward * TraceDistance;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AvatarCharacter);

	bool bHit = GetWorld()->LineTraceMultiByObjectType(
		HitResults,
		Start,
		End,
		FCollisionObjectQueryParams(ECC_Pawn),
		Params
	);

	// TODO: Debug 용
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor)
			{
				continue;
			}

			// TODO: 인터페이스 상속받은 적한테만 피격판정할꺼면 인터페이스 cast?
			UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>();
			if (!TargetASC)
			{
				continue;
			}

			FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			EffectContext.AddHitResult(Hit);
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
}
