#include "Characters/Enemy/F4EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AF4EnemyBase::AF4EnemyBase()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
		
		UBlackboardComponent* BlackboardComp = AIC->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsVector("HomeLocation", GetActorLocation());
		}
	}
}
