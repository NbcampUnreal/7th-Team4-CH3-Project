#include "Characters/Enemy/F4EnemyBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetEnemy.h"
#include "AIController.h"
#include "System/F4GameplayTags.h"

AF4EnemyBase::AF4EnemyBase()
{
	EnemyAttributeSet = CreateDefaultSubobject<UF4AttributeSetEnemy>(TEXT("EnemyAttributeSet"));
}

void AF4EnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AF4EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (ASC)
	{
		InitEnemyStats();
	}
	
	AAIController* AIC = Cast<AAIController>(NewController);
	if (AIC && BehaviorTree)
	{
		AIC->RunBehaviorTree(BehaviorTree);
	}
}

void AF4EnemyBase::InitEnemyStats()
{
	if (!ASC || !DefaultAttributeEffect)
	{
		return;
	}

	// 초기 스탯 적용 (Health, MaxHealth 등)
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, EffectContext);
	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
	// OnSpeedAttributeChanged(); -> BaseCharacter에서 처리해줘요 호출 필요 x 
}

void AF4EnemyBase::HandleDeath()
{
	// 1. 이미 죽었는지 태그로 확인 (중복 호출 방지)
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(F4GameplayTags::State_Dead))
	{
		return;
	}

	// 2. GA_Death를 실행할 태그 컨테이너 생성
	FGameplayTagContainer DeathTagContainer;
	DeathTagContainer.AddTag(F4GameplayTags::Ability_Death);

	// 3. 태그로 어빌리티 실행
	bool bSuccess = GetAbilitySystemComponent()->TryActivateAbilitiesByTag(DeathTagContainer);

	// 4. 만약 GA_Death 실행에 실패했다면 즉시 Destroy
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_Death 활성화 실패! 즉시 Destroy 합니다."));
		Destroy();
	}
}
