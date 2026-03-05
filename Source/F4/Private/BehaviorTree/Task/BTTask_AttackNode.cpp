#include "BehaviorTree/Task/BTTask_AttackNode.h"
#include "GameFramework/Character.h"
#include "System/F4GameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	if (Owner && BB)
	{
		AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
		if (TargetActor)
		{
			Owner->SetFocus(TargetActor); // 엔진이 목표 각도를 계산하도록 설정
		}
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
	
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (BlackBoard == nullptr)
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
	
	if (AICharacter && BlackBoard && Owner)
	{
		AActor* TargetActor = Cast<AActor>(BlackBoard->GetValueAsObject(FName("TargetActor")));
		if (TargetActor)
		{
			// 현재 회전값과 목표 회전값 계산
			FRotator CurrentRot = AICharacter->GetActorRotation();
			FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(
				AICharacter->GetActorLocation(),
				TargetActor->GetActorLocation()
				);
            
			// Yaw값만 회전 (기울어짐 방지)
			TargetRot.Pitch = 0.f;
			TargetRot.Roll = 0.f;

			// RInterpTo를 이용해 부드러운 회전값 계산
			// InterpSpeed 값이 높을수록 빠르게 회전 (5.0 ~ 10.0)
			float InterpSpeed = 10.f; 
			FRotator SoftRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, InterpSpeed);

			// 계산된 부드러운 회전 적용
			AICharacter->SetActorRotation(SoftRot);
		}
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
