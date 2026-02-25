#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/F4CharacterBase.h"
#include "Engine/DataTable.h"
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
	
	UPROPERTY(editAnywhere, BlueprintReadOnly, Category = "GAS | Data")
	FDataTableRowHandle CombatDataHandle;

	// 서비스에서 쉽게 가져다 쓸 수 있도록 만든 Getter
	FORCEINLINE FName GetEnemyRowName() const { return CombatDataHandle.RowName; }
	FORCEINLINE TObjectPtr<const UDataTable> GetCombatDataTable() const { return CombatDataHandle.DataTable; }
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	// 사용할 AI 로직
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsDead = false;

public:
	void SetIsDead(bool NewIsDead) { bIsDead = NewIsDead; }
	
protected:
	// 1. 에디터에서 선택할 무기 클래스 (근거리면 칼, 원거리면 총)
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> WeaponClass;

	// 2. 무기가 붙을 소켓 이름 (기본값 설정 가능)
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSocketName = TEXT("WeaponSocket");

	// 소환된 무기를 저장 (나중에 외형 변경 등이 필요할 때 사용)
	UPROPERTY()
	AActor* EquippedWeapon;

	// 무기 생성 및 부착 함수
	void SpawnDefaultWeapon();
};
