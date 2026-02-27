#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4QuickSlotBarWidget.generated.h"

class UF4ItemInstance;
class UF4QuickSlotComponent;
class UF4QuickSlotWidget;
class UHorizontalBox;

UCLASS()
class F4_API UF4QuickSlotBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnQuickSlotUpdatedCallback(int32 SlotIndex, UF4ItemInstance* ItemInstance);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> QuickSlotBox;

	UPROPERTY(EditDefaultsOnly, Category = "QuickSlot")
	TSubclassOf<UF4QuickSlotWidget> QuickSlotWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UF4QuickSlotWidget>> SlotWidgets;

	UPROPERTY()
	TObjectPtr<UF4QuickSlotComponent> QuickSlotComp;
};
