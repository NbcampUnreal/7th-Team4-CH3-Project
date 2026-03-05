#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4InventoryWidget.generated.h"

class UBorder;
class UWrapBox;
class UF4InventoryComponent;
class UF4InventorySlotWidget;

UCLASS()
class F4_API UF4InventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory | UI")
	void RefreshInventory();

	UPROPERTY(BlueprintReadWrite, Category = "Inventory | UI", meta = (ExposeOnSpawn = true))
	UF4InventoryComponent* InventoryComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory | UI")
	TSubclassOf<UF4InventorySlotWidget> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryGrid;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> InventoryWindow;

	bool bIsDragging = false;
	FVector2D DragOffset;
};
