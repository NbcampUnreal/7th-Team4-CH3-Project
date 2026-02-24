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
