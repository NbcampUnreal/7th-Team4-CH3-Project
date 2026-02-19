#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectTarget.generated.h"

UCLASS()
class F4_API UBTService_DetectTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectRadius = 1000.0f;
	
	UFUNCTION(BlueprintCallable)
	void ClearBlackboardValues(UBlackboardComponent* BB);
	
};
