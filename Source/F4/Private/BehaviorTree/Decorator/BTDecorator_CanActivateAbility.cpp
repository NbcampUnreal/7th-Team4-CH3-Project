#include "BehaviorTree/Decorator/BTDecorator_CanActivateAbility.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

UBTDecorator_CanActivateAbility::UBTDecorator_CanActivateAbility()
{
	NodeName = "Can Activate Ability";
}

bool UBTDecorator_CanActivateAbility::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory
	) const
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	if (!AIController)
	{
		return false;
	}
	
	APawn* OwningPawn = AIController->GetPawn();
	if (!OwningPawn)
	{
		return false;
	}
	
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OwningPawn);
	if (ASCInterface)
	{
		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
		if (ASC)
		{
			// 해당 쿨타임 태그가 없다면(False) 스킬 사용 가능(True)
			return !ASC->HasMatchingGameplayTag(CooldownTag);
		}
	}

	return false;
}
