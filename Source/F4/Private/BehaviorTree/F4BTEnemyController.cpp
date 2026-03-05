#include "BehaviorTree/F4BTEnemyController.h"

AF4BTEnemyController::AF4BTEnemyController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AF4BTEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
