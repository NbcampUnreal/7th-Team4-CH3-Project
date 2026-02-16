#include "BehaviorTree/Service/BTService_DetectTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/Attributes/F4AttributeSetEnemy.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("Detect Target");
	Interval = 0.5f;
	RandomDeviation = 0.1f;

	TargetActorKey.AddObjectFilter(
		this, 
		GET_MEMBER_NAME_CHECKED(UBTService_DetectTarget, TargetActorKey), AActor::StaticClass()
		);
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB) return;

	AF4EnemyBase* Enemy = Cast<AF4EnemyBase>(AIC->GetPawn());
	if (!Enemy || !Enemy->GetEnemyAttributeSet()) return;
	
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
    
	if (TargetActor == nullptr)
	{
		TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	
	if (TargetActor)
	{
		// 서비스 TickNode 내부 핵심 로직
		const float Distance = FVector::Dist(Enemy->GetActorLocation(), TargetActor->GetActorLocation());
		const float TraceRange = Enemy->GetEnemyAttributeSet()->GetTraceRange();
		const float AttackRange = Enemy->GetEnemyAttributeSet()->GetAttackRange();

		if (Distance <= TraceRange)
		{
			// 1. 타겟 세팅
			BB->SetValueAsObject(TargetActorKey.SelectedKeyName, TargetActor);
      
			// 2. 공격 사거리 판정
			bool bInAttackRange = (Distance <= AttackRange);
			BB->SetValueAsBool(TEXT("bIsInAttackRange"), bInAttackRange);
    
			// 3. 추적 사거리 판정
			bool bInTraceRange = (Distance <= TraceRange);
			BB->SetValueAsBool(TEXT("bIsInTraceRange"), bInTraceRange);
		}
		else
		{
			ClearBlackboardValues(BB);
		}
	}
}

void UBTService_DetectTarget::ClearBlackboardValues(UBlackboardComponent* BB)
{
	BB->ClearValue(TargetActorKey.SelectedKeyName);
	BB->SetValueAsBool(TEXT("bIsInTraceRange"), false);
	BB->SetValueAsBool(TEXT("bIsInAttackRange"), false);
}
