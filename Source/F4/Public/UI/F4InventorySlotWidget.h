#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4InventorySlotWidget.generated.h"

class UF4InventoryContextMenu;
class UImage;
class UF4ItemInstance;
class UTextBlock;
class UButton;

UCLASS()
class F4_API UF4InventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory | UI", meta = (ExposeOnSpawn = true))
	TObjectPtr<UF4ItemInstance> SlotItemInstance;

	UFUNCTION(BlueprintCallable, Category = "Inventory | UI")
	void UpdateSlotUI();

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory | UI")
	TSubclassOf<UF4InventoryContextMenu> ContextMenuClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantityText;
};
