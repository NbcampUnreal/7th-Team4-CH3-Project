#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanActivateAbility.generated.h"

UCLASS()
class F4_API UBTDecorator_CanActivateAbility : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CanActivateAbility();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GAS")
	FGameplayTag CooldownTag;		
};
