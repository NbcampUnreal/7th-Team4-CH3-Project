#include "BehaviorTree/Task/BTTask_AttackNode.h"
#include "GameFramework/Character.h"
#include "System/F4GameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"

UBTTask_AttackNode::UBTTask_AttackNode()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	if (Owner == nullptr)
	{
		return EBTNodeResult::Aborted;
	}
	
	ACharacter* AICharacter = Cast<ACharacter>(Owner->GetPawn());
	if (AICharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IAbilitySystemInterface* ASCHolder = Cast<IAbilitySystemInterface>(AICharacter);
	if (ASCHolder)
	{
		UAbilitySystemComponent* ASC = ASCHolder->GetAbilitySystemComponent();
		if (ASC)
		{
			// 등록된 태그를 통해 능력을 활성화
			if (ASC->TryActivateAbilitiesByTag(AttackTagContainer)) 
			{
				return EBTNodeResult::InProgress;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to Activate Ability by Tag!"));
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_AttackNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	if (Owner == nullptr)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}
	
	ACharacter* AICharacter = Cast<ACharacter>(Owner->GetPawn());
	if (AICharacter == nullptr)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}
	
	UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		return;
	}
	
	IAbilitySystemInterface* ASCHolder = Cast<IAbilitySystemInterface>(AICharacter);
	if (ASCHolder)
	{
		UAbilitySystemComponent* ASC = ASCHolder->GetAbilitySystemComponent();
    
		// 공격 중임을 나타내는 태그가 사라졌는지 확인
		if (!ASC->HasMatchingGameplayTag(F4GameplayTags::State_Attacking))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
