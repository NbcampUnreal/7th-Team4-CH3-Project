#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/F4CharacterBase.h"
#include "F4EnemyBase.generated.h"

class UF4AttributeSetEnemy;

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
	
protected:
	// 적에게만 필요한 추가 어트리뷰트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | Attributes")
	UF4AttributeSetEnemy* EnemyAttributeSet;
	
	// 에디터에서 설정할 초기 스탯용 GE (예: HP, 공격력 설정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS | Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// 사용할 AI 로직
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
	
	
	// 사망시 콜백함수
	void HandleDeath();
	
	//void SetMoveSpeed();
	
public:
	// FORCEINLINE은 성능을 위해 붙여주며, 외부에서 어트리뷰트 셋을 안전하게 읽게 해줌
	FORCEINLINE class UF4AttributeSetEnemy* GetEnemyAttributeSet() const { return EnemyAttributeSet; }
};
