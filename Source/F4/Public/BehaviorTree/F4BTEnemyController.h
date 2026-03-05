#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "F4BTEnemyController.generated.h"

UCLASS()
class F4_API AF4BTEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AF4BTEnemyController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
};
