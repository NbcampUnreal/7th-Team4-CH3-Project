#include "Characters/Enemy/F4EnemyBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes//F4AttributeSetEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AF4EnemyBase::AF4EnemyBase()
{
	EnemyAttributeSet = CreateDefaultSubobject<UF4AttributeSetEnemy>(TEXT("EnemyAttributeSet"));
}

void AF4EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyAttributeSet)
	{
		if (AAIController* AIC = Cast<AAIController>(GetController()))
		{
			if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
			{
				// 어트리뷰트셋의 초기값들을 블랙보드 키에 동기화
				BB->SetValueAsFloat(TEXT("AttackRange"), EnemyAttributeSet->GetAttackRange());
				BB->SetValueAsFloat(TEXT("TraceRange"), EnemyAttributeSet->GetTraceRange());
				BB->SetValueAsFloat(TEXT("MinTraceRange"), EnemyAttributeSet->GetMinTraceRange());
				BB->SetValueAsFloat(TEXT("AttackCooldown"), EnemyAttributeSet->GetAttackCooldown());

				UE_LOG(LogTemp, Log, TEXT("AI Blackboard Initialized with Attributes!"));
			}
		}
	}
}

void AF4EnemyBase::PossessedBy(AController* NewController)
{
	// 1. 부모의 PossessedBy를 먼저 실행 (Abilities 부여 등 수행)
	Super::PossessedBy(NewController);

	// 2. 자식 클래스 전용 설정
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this); 
		
		InitEnemyStats();
	}

	// 3. AI 트리 실행
	AAIController* AIC = Cast<AAIController>(NewController);
	if (AIC && BehaviorTree)
	{
		AIC->RunBehaviorTree(BehaviorTree);
	}
}

void AF4EnemyBase::InitEnemyStats()
{
	if (!ASC || !DefaultAttributeEffect) return;

	// 초기 스탯 적용 (Health, MaxHealth 등)
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, EffectContext);
	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
	OnSpeedAttributeChanged();
}

bool AF4EnemyBase::IsInAttackRange(AActor* TargetActor)
{
	if (!TargetActor || !EnemyAttributeSet) return false;
    
	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	
	return Distance <= EnemyAttributeSet->GetAttackRange();
}

bool AF4EnemyBase::IsInTraceRange(AActor* TargetActor)
{
	if (!TargetActor || !EnemyAttributeSet) return false;
    
	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	
	return Distance <= EnemyAttributeSet->GetTraceRange();
}

bool AF4EnemyBase::IsInMinTraceRange(AActor* TargetActor)
{
	if (!TargetActor || !EnemyAttributeSet) return false;
    
	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	
	return Distance <= EnemyAttributeSet->GetMinTraceRange();
}
