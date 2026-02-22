#include "Characters/Enemy/F4EnemyBase.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "System/F4GameplayTags.h"

AF4EnemyBase::AF4EnemyBase()
{
}

void AF4EnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AF4EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AAIController* AIC = Cast<AAIController>(NewController);
	if (AIC && BehaviorTree)
	{
		AIC->RunBehaviorTree(BehaviorTree);
	}
}


// todo: 사망 몽타주 실행하고 아이템을 드랍하도록 변경예정 >> 어빌리티 실행해서 어빌리티에서 모두 처리할꺼임
void AF4EnemyBase::HandleDeath()
{
	// 이미 죽었는지 태그로 확인
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(F4GameplayTags::State_Dead))
	{
		return;
	}

	// GA_Death를 실행할 태그 컨테이너 생성
	FGameplayTagContainer DeathTagContainer;
	DeathTagContainer.AddTag(F4GameplayTags::Ability_Death);

	// 태그로 어빌리티 실행
	bool bSuccess = GetAbilitySystemComponent()->TryActivateAbilitiesByTag(DeathTagContainer);

	// GA_Death 실행에 실패했다면 즉시 Destroy
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_Death 활성화 실패! 즉시 Destroy 합니다."));
		Destroy();
	}
}
