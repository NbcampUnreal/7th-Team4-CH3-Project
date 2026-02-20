#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "F4ItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EF4ItemType : uint8
{
	Weapon		UMETA(DisplayName = "Weapon"),
	Consumable	UMETA(DisplayName = "Consumable")
};

UCLASS(Abstract)
class F4_API UF4ItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// 기본 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Info")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Info")
	EF4ItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Visual")
	TObjectPtr<UStaticMesh> PickupMesh;;
};
