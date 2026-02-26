#include "UI/F4QuickSlotBarWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Inventory/F4QuickSlotComponent.h"
#include "UI/F4QuickSlotWidget.h"

void UF4QuickSlotBarWidget::OnQuickSlotUpdatedCallback(int32 SlotIndex, UF4ItemInstance* ItemInstance)
{
	if (SlotWidgets.IsValidIndex(SlotIndex))
	{
		SlotWidgets[SlotIndex]->UpdateSlotUI(ItemInstance);
	}
}

void UF4QuickSlotBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		QuickSlotComp = OwningPawn->FindComponentByClass<UF4QuickSlotComponent>();
	}

	if (QuickSlotComp && QuickSlotBox && QuickSlotWidgetClass)
	{
		QuickSlotBox->ClearChildren();
		SlotWidgets.Empty();

		for (int32 i = 0; i < 8; ++i)
		{
			UF4QuickSlotWidget* NewSlot = CreateWidget<UF4QuickSlotWidget>(GetWorld(), QuickSlotWidgetClass);
			if (NewSlot)
			{
				NewSlot->SlotIndex = i;
				UHorizontalBoxSlot* BoxSlot = QuickSlotBox->AddChildToHorizontalBox(NewSlot);

				if (BoxSlot)
				{
					BoxSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
				}

				SlotWidgets.Add(NewSlot);
			}
		}

		QuickSlotComp->OnQuickSlotUpdated.AddDynamic(this, &UF4QuickSlotBarWidget::OnQuickSlotUpdatedCallback);
	}
}
