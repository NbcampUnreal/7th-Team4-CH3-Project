#include "UI/F4InventorySlotWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemFragment_Stackable.h"
#include "GameFramework/Pawn.h"
#include "UI/F4InventoryContextMenu.h"

void UF4InventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UpdateSlotUI();
}

FReply UF4InventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        if (ContextMenuClass && SlotItemInstance)
        {
            UF4InventoryContextMenu* ContextMenu = CreateWidget<UF4InventoryContextMenu>(GetWorld(), ContextMenuClass);

            if (ContextMenu)
            {
                ContextMenu->InitMenu(SlotItemInstance);
                ContextMenu->AddToViewport(100);

                FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
                ContextMenu->SetPositionInViewport(MousePosition);

                return FReply::Handled();
            }
        }
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UF4InventorySlotWidget::UpdateSlotUI()
{
    if (!SlotItemInstance || !SlotItemInstance->ItemDefinition)
    {
        return;
    }

    if (const UF4ItemFragment_UI* UIFragment = SlotItemInstance->ItemDefinition->FindFragmentByClass<UF4ItemFragment_UI>())
    {
        if (ItemIcon && UIFragment->ItemIcon)
        {
            ItemIcon->SetBrushFromTexture(UIFragment->ItemIcon);
        }

        if (ItemNameText)
        {
            ItemNameText->SetText(UIFragment->ItemName);
        }
    }

    if (ItemQuantityText)
    {
        const UF4ItemFragment_Stackable* StackFragment = SlotItemInstance->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Stackable>();

        if (StackFragment && StackFragment->MaxStackSize > 1)
        {
            FString QuantityString = FString::Printf(TEXT("x%d"), SlotItemInstance->Quantity);
            ItemQuantityText->SetText(FText::FromString(QuantityString));
            ItemQuantityText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }
        else
        {
            ItemQuantityText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
