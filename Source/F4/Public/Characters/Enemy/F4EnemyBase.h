#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/F4CharacterBase.h"
#include "F4EnemyBase.generated.h"

class UF4EnemyAttributeSet;

UCLASS()
class F4_API AF4EnemyBase : public AF4CharacterBase
{
	GENERATED_BODY()

public:
	AF4EnemyBase();

	// 로컬 게임이라도 AI Controller가 빙의되는 시점에 초기화하는 것이 가장 안전
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

	// GAS 및 초기 데이터 세팅
	void InitEnemyStats();
	
public:
	UFUNCTION(BlueprintCallable, Category = "F4 | Combat")
	bool IsInAttackRange(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, Category = "F4 | Combat")
	bool IsInTraceRange(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, Category = "F4 | Combat")
	bool IsInMinTraceRange(AActor* TargetActor);

protected:
	// 적에게만 필요한 추가 어트리뷰트 (AttackRange, Cooldown 등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "F4 | GAS | Attributes")
	UF4EnemyAttributeSet* EnemyAttributeSet;
	// 에디터에서 설정할 초기 스탯용 GE (예: HP, 공격력 설정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "F4 | GAS | Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// 사용할 AI 로직
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "F4 | AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};