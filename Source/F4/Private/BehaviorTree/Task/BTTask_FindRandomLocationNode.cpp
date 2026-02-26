#include "BehaviorTree/Task/BTTask_FindRandomLocationNode.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocationNode::UBTTask_FindRandomLocationNode()
{
	NodeName = TEXT("Find Random LocationNode");
	
	PatrolLocationKey.AddVectorFilter(
		this, 
		GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocationNode, PatrolLocationKey)
		);
}

EBTNodeResult::Type UBTTask_FindRandomLocationNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AiController = OwnerComp.GetAIOwner();
	if (AiController == nullptr || AiController->GetPawn()== nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BlackboardCom = OwnerComp.GetBlackboardComponent();
	
	const FVector Origin = BlackboardCom->GetValueAsVector(HomeLocationKey.SelectedKeyName);
	FNavLocation RandomLocation;
	
	const bool bFound = NavSystem->GetRandomReachablePointInRadius(Origin, SearchRadius, RandomLocation);
	
	if (bFound)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			PatrolLocationKey.SelectedKeyName,
			RandomLocation.Location
			);
		
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
