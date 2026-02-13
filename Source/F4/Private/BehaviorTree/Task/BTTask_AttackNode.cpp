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
	
	// 나중에 여기서 GA_Attack 호출로 변경
	//AICharacter->PlayAnimMontage(AttackMontage);

	IAbilitySystemInterface* ASCHolder = Cast<IAbilitySystemInterface>(AICharacter);
	if (ASCHolder)
	{
		UAbilitySystemComponent* ASC = ASCHolder->GetAbilitySystemComponent();
		if (ASC)
		{
			// 2. 등록된 태그(예: "Ability.Attack")를 통해 능력을 활성화합니다.
			// (AttackTag는 헤더에 변수로 선언해두면 좋습니다)
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
    
		// 공격 중임을 나타내는 태그(예: State.Attacking)가 사라졌는지 확인
		// 생성자에서 설정하신 F4GameplayTags::State_Attacking를 사용하세요.
		if (!ASC->HasMatchingGameplayTag(F4GameplayTags::State_Attacking))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
