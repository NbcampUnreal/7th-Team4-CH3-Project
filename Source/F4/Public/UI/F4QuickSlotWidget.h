#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4QuickSlotWidget.generated.h"

class UTextBlock;
class UImage;
class UF4ItemInstance;

UCLASS()
class F4_API UF4QuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuickSlot")
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuickSlot")
	TObjectPtr<UF4ItemInstance> BoundItem;

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void UpdateSlotUI(UF4ItemInstance* NewItem);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void UpdateSelectionBorder(UF4ItemInstance* ActiveItem);

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnQuickSlotUpdatedCallback(int32 InSlotIndex, UF4ItemInstance* InItem);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantityText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HotkeyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectionBorder;
};
