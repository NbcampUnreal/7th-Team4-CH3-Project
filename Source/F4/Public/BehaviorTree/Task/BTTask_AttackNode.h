#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackNode.generated.h"

UCLASS()
class F4_API UBTTask_AttackNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackNode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// 에디터에서 공격에 필요한 태그를 선택할 수 있게 합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	FGameplayTagContainer AttackTagContainer;
};
