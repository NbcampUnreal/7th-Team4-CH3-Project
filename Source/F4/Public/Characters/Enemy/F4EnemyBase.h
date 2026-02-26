#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/F4CharacterBase.h"
#include "Engine/DataTable.h"
#include "F4EnemyBase.generated.h"

class UF4AttributeSetEnemy;

UENUM(BlueprintType)
enum class EEnemyType : uint8 { Melee, Ranged };

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsDead = false;

public:
	void SetIsDead(bool NewIsDead) { bIsDead = NewIsDead; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> WeaponClass;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSocketName = TEXT("WeaponSocket");
	
	UPROPERTY()
	AActor* EquippedWeapon;
	
	void SpawnDefaultWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAiming = false;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	EEnemyType EnemyType;
	
	void SetIsAiming(bool bNewState) { bIsAiming = bNewState; }
    
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool GetIsAiming() const { return bIsAiming; }
};
