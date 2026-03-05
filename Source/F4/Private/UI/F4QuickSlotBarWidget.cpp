#include "UI/F4QuickSlotBarWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Inventory/F4QuickSlotComponent.h"
#include "UI/F4QuickSlotWidget.h"
#include "UI/F4WeaponSlotWidget.h"

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

	if (!QuickSlotBox)
	{
		return;
	}

	QuickSlotBox->ClearChildren();
	WeaponSlotWidgets.Empty();
	SlotWidgets.Empty();

	// 무기 슬롯 위젯 2개 생성 (Primary=0, Secondary=1)
	if (WeaponSlotWidgetClass)
	{
		for (int32 i = 0; i < 2; ++i)
		{
			UF4WeaponSlotWidget* WeaponWidget = CreateWidget<UF4WeaponSlotWidget>(GetOwningPlayer(), WeaponSlotWidgetClass);
			if (WeaponWidget)
			{
				WeaponWidget->SlotIndex = i;
				UHorizontalBoxSlot* BoxSlot = QuickSlotBox->AddChildToHorizontalBox(WeaponWidget);
				if (BoxSlot)
				{
					BoxSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
				}
				WeaponSlotWidgets.Add(WeaponWidget);
			}
		}
	}

	// 소비 아이템 퀵슬롯 위젯 생성
	if (QuickSlotComp && QuickSlotWidgetClass)
	{
		for (int32 i = 0; i < UF4QuickSlotComponent::TotalSlotCount; ++i)
		{
			UF4QuickSlotWidget* NewSlot = CreateWidget<UF4QuickSlotWidget>(GetOwningPlayer(), QuickSlotWidgetClass);
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
