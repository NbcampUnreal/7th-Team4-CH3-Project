#include "UI/F4QuickSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4QuickSlotComponent.h"

void UF4QuickSlotWidget::UpdateSlotUI(UF4ItemInstance* NewItem)
{
	BoundItem = NewItem;

	if (!BoundItem || !BoundItem->ItemDefinition)
	{
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		if (ItemQuantityText)
		{
			ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
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
}

void UF4QuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HotkeyText)
	{
		// 소비 슬롯 단축키: 키 3(슬롯0)부터 시작
		HotkeyText->SetText(FText::AsNumber(SlotIndex + 3));
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

	UpdateSlotUI(nullptr);
}

FReply UF4QuickSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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
