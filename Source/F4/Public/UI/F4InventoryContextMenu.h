#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4InventoryContextMenu.generated.h"

class UButton;
class UF4ItemInstance;

UCLASS()
class F4_API UF4InventoryContextMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitMenu(UF4ItemInstance* InItemInstance);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UFUNCTION()
	void OnEquipClicked();

	UFUNCTION()
	void OnDropClicked();

	UFUNCTION()
	void OnQuickSlotClicked();

	UFUNCTION()
	void OnUnregisterQuickSlotClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquipButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DropButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuickSlotButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UnregisterQuickSlotButton;

private:
	UPROPERTY()
	TObjectPtr<UF4ItemInstance> TargetItemInstance;
};
