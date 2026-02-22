#include "BehaviorTree/Service/BTService_DetectTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DataTable/EnemyData.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "System/F4GameplayTags.h"

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
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BlackBoard) return;

	AF4EnemyBase* Enemy = Cast<AF4EnemyBase>(AIC->GetPawn());
	if (!Enemy) return;
	
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return;
	
	// 데이터 테이블, Row Name을 현재 Pawn Owner에서 가져오기
	const UDataTable* DataTable = Enemy->GetCombatDataTable();
	FName RowName = Enemy->GetEnemyRowName();
	
	float AttackRange = 150.f;
	float TraceRange = 1500.f;
	
	if (DataTable && !RowName.IsNone())
	{
		static const FString ContextString(TEXT("CombatDataContext"));
		const FEnemyData* DataRow = DataTable->FindRow<FEnemyData>(RowName, ContextString);
		
		if (DataRow)
		{
			AttackRange = DataRow->AttackRange;
			TraceRange = DataRow->TraceRange;
		}
	}
	
	AActor* TargetActor = Cast<AActor>(BlackBoard->GetValueAsObject(TargetActorKey.SelectedKeyName));
    
	if (TargetActor == nullptr)
	{
		TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	
	if (TargetActor)
	{
		// 서비스 TickNode 내부 핵심 로직
		const float Distance = FVector::Dist(Enemy->GetActorLocation(), TargetActor->GetActorLocation());

		if (Distance <= TraceRange)
		{
			// 1. 타겟 세팅
			BlackBoard->SetValueAsObject(TargetActorKey.SelectedKeyName, TargetActor);
      
			// 2. 공격 사거리 판정
			bool bInAttackRange = (Distance <= AttackRange);
			BlackBoard->SetValueAsBool(TEXT("bIsInAttackRange"), bInAttackRange);
    
			// 3. 추적 사거리 판정
			bool bInTraceRange = (Distance <= TraceRange);
			BlackBoard->SetValueAsBool(TEXT("bIsInTraceRange"), bInTraceRange);
		}
		else
		{
			ClearBlackboardValues(BlackBoard);
		}
	}
}

void UBTService_DetectTarget::ClearBlackboardValues(UBlackboardComponent* BB)
{
	BB->ClearValue(TargetActorKey.SelectedKeyName);
	BB->SetValueAsBool(TEXT("bIsInTraceRange"), false);
	BB->SetValueAsBool(TEXT("bIsInAttackRange"), false);
}
