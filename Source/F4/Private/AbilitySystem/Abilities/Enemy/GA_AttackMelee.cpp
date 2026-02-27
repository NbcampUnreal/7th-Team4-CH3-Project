#include "AbilitySystem/Abilities/Enemy/GA_AttackMelee.h"
#include"AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_AttackMelee::UGA_AttackMelee()
{
}

void UGA_AttackMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	
    MontageTask->OnCompleted.AddDynamic(this, &UGA_AttackMelee::OnMontageCompleted);
    MontageTask->OnInterrupted.AddDynamic(this, &UGA_AttackMelee::OnMontageCancelled);
    MontageTask->OnCancelled.AddDynamic(this, &UGA_AttackMelee::OnMontageCancelled);
	
    MontageTask->ReadyForActivation();
	
    UAbilityTask_WaitGameplayEvent* WaitEventTask =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
            this,
            FGameplayTag::RequestGameplayTag(FName("Event.Montage.DealDamage"))
        );
	
    if (WaitEventTask)
    {
        WaitEventTask->EventReceived.AddDynamic(this, &UGA_AttackMelee::OnDamageGameplayEvent);
        WaitEventTask->ReadyForActivation();
    }
}

void UGA_AttackMelee::OnDamageGameplayEvent(FGameplayEventData EventData)
{
    PerformMeleeTrace();
}

void UGA_AttackMelee::PerformMeleeTrace()
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
    TSet<AActor*> UniqueActors;
    
    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor)
            {
                continue;
            }
            if (UniqueActors.Contains(HitActor))
            {
                continue;
            }
            UniqueActors.Add(HitActor);
            
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
