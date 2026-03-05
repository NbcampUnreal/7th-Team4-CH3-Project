#include "UI/F4WeaponSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemInstance.h"

void UF4WeaponSlotWidget::UpdateSlotUI(UF4ItemInstance* NewItem)
{
	BoundItem = NewItem;

	if (!BoundItem || !BoundItem->ItemDefinition)
	{
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		UpdateSelectionBorder(nullptr);
		return;
	}

	const UF4ItemFragment_UI* UIFragment = BoundItem->ItemDefinition->FindFragmentByClass<UF4ItemFragment_UI>();
	if (UIFragment && ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(UIFragment->ItemIcon);
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

void UF4WeaponSlotWidget::UpdateSelectionBorder(UF4ItemInstance* ActiveItem)
{
	if (!SelectionBorder)
	{
		return;
	}

	const bool bIsActive = (BoundItem != nullptr) && (BoundItem == ActiveItem);
	SelectionBorder->SetVisibility(bIsActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UF4WeaponSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HotkeyText)
	{
		HotkeyText->SetText(FText::AsNumber(SlotIndex + 1));
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	if (UF4EquipmentComponent* EquipmentComp = OwningPawn->FindComponentByClass<UF4EquipmentComponent>())
	{
		EquipmentComp->OnWeaponEquippedToSlot.AddDynamic(this, &ThisClass::OnWeaponSlotChangedCallback);
		EquipmentComp->OnActiveWeaponChanged.AddUniqueDynamic(this, &ThisClass::UpdateSelectionBorder);
	}

	UpdateSlotUI(nullptr);
}

FReply UF4WeaponSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() != EKeys::RightMouseButton)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	if (UF4EquipmentComponent* EquipmentComp = OwningPawn->FindComponentByClass<UF4EquipmentComponent>())
	{
		EquipmentComp->UnequipItemFromSlot(static_cast<EWeaponSlot>(SlotIndex));
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UF4WeaponSlotWidget::OnWeaponSlotChangedCallback(int32 InSlotIndex, UF4ItemInstance* InItem)
{
	if (SlotIndex == InSlotIndex)
	{
		UpdateSlotUI(InItem);
	}
}
