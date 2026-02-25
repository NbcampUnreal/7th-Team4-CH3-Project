#include "UI/F4InventoryWidget.h"

#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WrapBox.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemInstance.h"
#include "UI/F4InventorySlotWidget.h"

void UF4InventoryWidget::RefreshInventory()
{
	if (!InventoryComponent || !InventoryGrid || !InventorySlotClass)
	{
		return;
	}

	InventoryGrid->ClearChildren();

	for (UF4ItemInstance* Item : InventoryComponent->GetInventoryItems())
	{
		if (!Item)
		{
			continue;
		}

		UF4InventorySlotWidget* NewSlot = CreateWidget<UF4InventorySlotWidget>(GetWorld(), InventorySlotClass);
		if (NewSlot)
		{
			NewSlot->SlotItemInstance = Item;
			NewSlot->UpdateSlotUI();
			InventoryGrid->AddChildToWrapBox(NewSlot);
		}
	}
}

FReply UF4InventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InventoryWindow && InventoryWindow->IsHovered())
		{
			bIsDragging = true;

			UCanvasPanelSlot* WindowSlot = Cast<UCanvasPanelSlot>(InventoryWindow->Slot);
			if (WindowSlot)
			{
				FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
				DragOffset = LocalMousePos - WindowSlot->GetPosition();
			}

			return FReply::Handled().CaptureMouse(TakeWidget());
		}
	}
	return FReply::Unhandled();
}

FReply UF4InventoryWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsDragging)
	{
		bIsDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

FReply UF4InventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bIsDragging && InventoryWindow)
	{
		UCanvasPanelSlot* WindowSlot = Cast<UCanvasPanelSlot>(InventoryWindow->Slot);
		if (WindowSlot)
		{
			FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
			WindowSlot->SetPosition(LocalMousePos - DragOffset);
		}
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UF4InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		InventoryComponent = OwningPawn->FindComponentByClass<UF4InventoryComponent>();
	}

	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UF4InventoryWidget::RefreshInventory);
	}

	RefreshInventory();
}
