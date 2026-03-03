#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "Inventory/F4ItemDefinition.h"
#include "F4GameInstance.generated.h"

class AF4PlayerCharacter;
class UF4InventoryComponent;
class UF4EquipmentComponent;
class UF4QuickSlotComponent;
class UF4ItemInstance;

USTRUCT()
struct FItemSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UF4ItemDefinition> ItemDefinition = nullptr;

	UPROPERTY()
	int32 Quantity = 0;

	UPROPERTY()
	TMap<FGameplayTag, float> DynamicStats;
};

USTRUCT()
struct FInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHasSavedData = false;

	// 인벤토리 전체 아이템 (순서 유지)
	UPROPERTY()
	TArray<FItemSaveData> InventoryItems;

	// 무기 슬롯 → InventoryItems 인덱스 (-1 = 비어있음)
	UPROPERTY()
	int32 PrimaryWeaponIndex = -1;

	UPROPERTY()
	int32 SecondaryWeaponIndex = -1;

	// 퀵슬롯 6칸 → InventoryItems 인덱스 (-1 = 비어있음)
	UPROPERTY()
	TArray<int32> QuickSlotIndices;

	// 현재 활성 무기 슬롯 (EWeaponSlot을 uint8로 저장)
	UPROPERTY()
	uint8 ActiveWeaponSlot = 255;
};


UCLASS()
class F4_API UF4GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	void WipeData();
	
	void SaveData(AF4PlayerCharacter* PlayerCharacter);

	void RestoreData(AF4PlayerCharacter* PlayerCharacter);

	bool HasSavedData() const { return SavedInventory.bHasSavedData; }

	// 사망으로 인한 맵 전환임을 표시 → EndPlay에서 SaveData 건너뜀
	void MarkDeathTransition() { bIsDeathTransition = true; }
	bool IsDeathTransition() const { return bIsDeathTransition; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowDifficultyScaling = true;
	UPROPERTY()
	int32 PermanentDifficulty = 0;

	UFUNCTION(BlueprintCallable, Category="F4|Difficulty")
	void IncrementPermanentDifficulty();

private:
	TMap<UF4ItemInstance*, int32> SaveInventoryItems(UF4InventoryComponent* InventoryComp);
	void SaveEquipmentSlots(UF4EquipmentComponent* EquipmentComp, const TMap<UF4ItemInstance*, int32>& ItemIndexMap);
	void SaveQuickSlots(UF4QuickSlotComponent* QuickSlotComp, const TMap<UF4ItemInstance*, int32>& ItemIndexMap);

	TArray<UF4ItemInstance*> RestoreInventoryItems(UF4InventoryComponent* InventoryComp, AActor* Outer);
	void RestoreEquipmentSlots(UF4EquipmentComponent* EquipmentComp, const TArray<UF4ItemInstance*>& RestoredItems);
	void RestoreQuickSlots(UF4QuickSlotComponent* QuickSlotComp, const TArray<UF4ItemInstance*>& RestoredItems);

	UPROPERTY()
	FInventorySaveData SavedInventory;

	bool bIsDeathTransition = false;
};
