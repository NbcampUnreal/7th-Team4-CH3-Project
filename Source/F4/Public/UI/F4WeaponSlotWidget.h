#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4WeaponSlotWidget.generated.h"

class UTextBlock;
class UImage;
class UF4ItemInstance;

UCLASS()
class F4_API UF4WeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponSlot")
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponSlot")
	TObjectPtr<UF4ItemInstance> BoundItem;

	UFUNCTION(BlueprintCallable, Category = "WeaponSlot")
	void UpdateSlotUI(UF4ItemInstance* NewItem);

	UFUNCTION(BlueprintCallable, Category = "WeaponSlot")
	void UpdateSelectionBorder(UF4ItemInstance* ActiveItem);

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnWeaponSlotChangedCallback(int32 InSlotIndex, UF4ItemInstance* InItem);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HotkeyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectionBorder;
};
