#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomLocationNode.generated.h"

/**
 * 
 */
UCLASS()
class F4_API UBTTask_FindRandomLocationNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindRandomLocationNode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere,Category="Blackboard")
	FBlackboardKeySelector HomeLocationKey;
	UPROPERTY(EditAnywhere,Category="Blackboard")
	FBlackboardKeySelector PatrolLocationKey;
	
	UPROPERTY(EditAnywhere,Category="AI")
	float SearchRadius = 1000.0f;
};
