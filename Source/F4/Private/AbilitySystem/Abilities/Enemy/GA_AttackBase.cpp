#include "AbilitySystem/Abilities/Enemy/GA_AttackBase.h"
#include"AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "System/F4GameplayTags.h"

UGA_AttackBase::UGA_AttackBase()
{
	// 1. 이 어빌리티의 이름표
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Attack));

	// 2. 실행 중일 때 캐릭터에게 부여될 태그 (다른 곳에서 상태 확인용)
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Attacking);

	// 3. 실행을 막는 조건들
	ActivationBlockedTags.AddTag(F4GameplayTags::Ability_Death);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Attacking);
	
	
}

void UGA_AttackBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
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
	
	MontageTask->OnCompleted.AddDynamic(this, &UGA_AttackBase::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_AttackBase::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_AttackBase::OnMontageCancelled);
	
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			FGameplayTag::RequestGameplayTag(FName("Event.Montage.DealDamage"))
		);

	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_AttackBase::OnDamageGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_AttackBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_AttackBase::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_AttackBase::OnDamageGameplayEvent(FGameplayEventData EventData)
{
	PerformMeleeTrace();
}

// 데미지 적용 범위 디버그
void UGA_AttackBase::PerformMeleeTrace()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
    if (!bShowdebug || !AvatarActor)
    {
        return;
    }

    // ===== Trace 시작/끝 위치 계산 =====
    FVector Start = AvatarActor->GetActorLocation();
    FVector Forward = AvatarActor->GetActorForwardVector();
    FVector End = Start + Forward * TraceDistance;

    // ===== Sphere Trace 수행 =====
    TArray<FHitResult> HitResults;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(AvatarActor);

    bool bHit = GetWorld()->SweepMultiByObjectType(
        HitResults,
        Start,
        End,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_Pawn),
        FCollisionShape::MakeSphere(TraceRadius),
        Params
    );

    // ===== Debug Draw (개발용) =====
    DrawDebugSphere(GetWorld(), Start, TraceRadius, 12, FColor::Red, false, 1.0f);
    DrawDebugSphere(GetWorld(), End, TraceRadius, 12, FColor::Red, false, 1.0f);

    // ===== Hit된 Actor들에게 Damage Effect 적용 =====
    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor)
            {
                continue;
            }

            // Hit Actor의 ASC 가져오기
            UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>();
            if (!TargetASC)
            {
                continue;
            }

            // Damage Effect 적용
            FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
            EffectContext.AddSourceObject(AvatarActor);

            FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
                DamageEffectClass,
                1.0f, // Level
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

