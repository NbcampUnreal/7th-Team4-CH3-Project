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

	if (BoundItem && BoundItem->ItemDefinition)
	{
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
	else
	{
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}

		if (ItemQuantityText)
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
		FString KeyString = FString::FromInt(SlotIndex + 1);
		HotkeyText->SetText(FText::FromString(KeyString));
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
		QuickSlotComp->UnregisterItem(SlotIndex);
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
