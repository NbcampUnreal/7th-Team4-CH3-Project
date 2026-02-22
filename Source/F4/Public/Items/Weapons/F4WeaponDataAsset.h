#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Items/F4ItemDataAsset.h"
#include "F4WeaponDataAsset.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FWeaponAbilityConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag InputTag; 
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee	UMETA(DisplayName = "Melee"),
	Gun		UMETA(DisplayName = "Gun")
};

UCLASS()
class F4_API UF4WeaponDataAsset : public UF4ItemDataAsset
{
	GENERATED_BODY()

public:
	UF4WeaponDataAsset();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	// 총기일 경우에만 편집 가능하도록 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Visual",
		meta = (EditCondition = "WeaponType == EWeaponType::Gun"))
	TObjectPtr<UStaticMesh> MagazineMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Info")
	EWeaponType WeaponType;

	// 총몸에 만들어둔 소켓 이름 (예: "Socket_Magazine")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual",
		meta = (EditCondition = "WeaponType == EWeaponType::Gun"))
	FName MagazineSocketName;
	
	// 총구 소켓 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual",
		meta = (EditCondition = "WeaponType == EWeaponType::Gun"))
	FName MuzzleSocketName;

	// 3. GAS (능력 및 스탯)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|GAS")
	TArray<FWeaponAbilityConfig> Abilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|GAS")
	TMap<FGameplayTag, float> BaseStats; // 초기 스탯 (예: Damage.Base = 50)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|GAS")
	TSubclassOf<UGameplayEffect> InitStatsEffect;
};
