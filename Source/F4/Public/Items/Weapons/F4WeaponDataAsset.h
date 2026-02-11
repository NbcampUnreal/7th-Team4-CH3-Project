#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "F4WeaponDataAsset.generated.h"

class UGameplayAbility;
class UAttributeSet;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Gun UMETA(DisplayName = "Gun")
};

UCLASS()
class F4_API UF4WeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 1. 기본 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Info")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Info")
	EWeaponType WeaponType;

	// 2. 비주얼 (총기는 MainMesh + AttachmentMesh로 구성 가능)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual")
	TObjectPtr<UStaticMesh> MainMesh; // 총몸 혹은 칼

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual",
		meta = (EditCondition = "WeaponType == EWeaponType::Gun"))
	TObjectPtr<UStaticMesh> AttachmentMesh; // 탄창, 스코프 등 추가 파츠

	// 3. GAS (능력 및 스탯)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|GAS")
	TSubclassOf<UGameplayAbility> AttackAbility; // 주 공격 능력 (발사/휘두르기)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|GAS")
	TMap<FGameplayTag, float> BaseStats; // 초기 스탯 (예: Damage.Base = 50)
};
