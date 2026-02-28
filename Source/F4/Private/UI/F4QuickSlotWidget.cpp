#include "UI/F4QuickSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4QuickSlotComponent.h"

void UF4QuickSlotWidget::UpdateSlotUI(UF4ItemInstance* NewItem)
{
	BoundItem = NewItem;

	if (!BoundItem || !BoundItem->ItemDefinition)
	{
		if (ItemIcon) ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		if (ItemQuantityText) ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
		UpdateSelectionBorder(nullptr);
		return;
	}

	const UF4ItemFragment_UI* UIFragment = BoundItem->ItemDefinition->FindFragmentByClass<UF4ItemFragment_UI>();
	if (UIFragment && ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(UIFragment->ItemIcon);
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (ItemQuantityText)
	{
		if (BoundItem->Quantity > 1)
		{
			ItemQuantityText->SetText(FText::AsNumber(BoundItem->Quantity));
			ItemQuantityText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	if (UF4EquipmentComponent* EquipmentComp = OwningPawn->FindComponentByClass<UF4EquipmentComponent>())
	{
		UpdateSelectionBorder(EquipmentComp->GetActiveWeaponInstance());
	}
}

void UF4QuickSlotWidget::UpdateSelectionBorder(UF4ItemInstance* ActiveItem)
{
	if (!SelectionBorder)
	{
		return;
	}

	const bool bIsActive = (BoundItem != nullptr) && (BoundItem == ActiveItem);
	SelectionBorder->SetVisibility(bIsActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UF4QuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HotkeyText)
	{
		FString KeyString = FString::FromInt(SlotIndex + 1);
		HotkeyText->SetText(FText::FromString(KeyString));
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	if (UF4QuickSlotComponent* QuickSlotComp = OwningPawn->FindComponentByClass<UF4QuickSlotComponent>())
	{
		QuickSlotComp->OnQuickSlotUpdated.AddDynamic(this, &ThisClass::OnQuickSlotUpdatedCallback);
	}

	if (UF4EquipmentComponent* EquipComp = OwningPawn->FindComponentByClass<UF4EquipmentComponent>())
	{
		EquipComp->OnActiveWeaponChanged.AddUniqueDynamic(this, &ThisClass::UpdateSelectionBorder);
	}

	UpdateSlotUI(nullptr);
}

FReply UF4QuickSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() != EKeys::RightMouseButton)
	{
		return Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	}

	if (UF4QuickSlotComponent* QuickSlotComp = OwningPawn->FindComponentByClass<UF4QuickSlotComponent>())
	{
		QuickSlotComp->ClearSlot(SlotIndex);
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UF4QuickSlotWidget::OnQuickSlotUpdatedCallback(int32 InSlotIndex, UF4ItemInstance* InItem)
{
	if (SlotIndex == InSlotIndex)
	{
		UpdateSlotUI(InItem);
	}
}
