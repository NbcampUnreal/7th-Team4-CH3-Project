#include "UI/F4InventoryContextMenu.h"

#include "Components/Button.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Consumable.h"
#include "Inventory/F4ItemFragment_Equipment.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4QuickSlotComponent.h"

void UF4InventoryContextMenu::InitMenu(UF4ItemInstance* InItemInstance)
{
	TargetItemInstance = InItemInstance;
	if (!TargetItemInstance || !TargetItemInstance->ItemDefinition) return;

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	UF4QuickSlotComponent* QuickSlotComp = OwningPawn->FindComponentByClass<UF4QuickSlotComponent>();

	const bool bIsEquippable = TargetItemInstance->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Equipment>() != nullptr;
	const bool bIsConsumable = TargetItemInstance->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Consumable>() != nullptr;

	bool bIsAlreadyInQuickSlot = false;
	if (QuickSlotComp)
	{
		bIsAlreadyInQuickSlot = (QuickSlotComp->FindItemSlotIndex(TargetItemInstance) != -1);
	}

	bool bHasSameType = false;
	if (QuickSlotComp)
	{
		for (int32 i = 0; i < 8; ++i)
		{
			UF4ItemInstance* SlotItem = QuickSlotComp->GetItemAtIndex(i);
			if (SlotItem && SlotItem->ItemDefinition == TargetItemInstance->ItemDefinition)
			{
				bHasSameType = true;
				break;
			}
		}
	}

	if (EquipButton)
	{
		EquipButton->SetVisibility(bIsEquippable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (QuickSlotButton)
	{
		QuickSlotButton->SetVisibility(bIsConsumable && !bIsAlreadyInQuickSlot && !bHasSameType ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (UnregisterQuickSlotButton)
	{
		UnregisterQuickSlotButton->SetVisibility(bIsAlreadyInQuickSlot ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UF4InventoryContextMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	if (EquipButton)
	{
		EquipButton->OnClicked.AddDynamic(this, &UF4InventoryContextMenu::OnEquipClicked);
	}

	if (DropButton)
	{
		DropButton->OnClicked.AddDynamic(this, &UF4InventoryContextMenu::OnDropClicked);
	}

	if (QuickSlotButton)
	{
		QuickSlotButton->OnClicked.AddDynamic(this, &UF4InventoryContextMenu::OnQuickSlotClicked);
	}

	if (UnregisterQuickSlotButton)
	{
		UnregisterQuickSlotButton->OnClicked.AddDynamic(this, &UF4InventoryContextMenu::OnUnregisterQuickSlotClicked);
	}
}

void UF4InventoryContextMenu::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	RemoveFromParent();
}

void UF4InventoryContextMenu::OnEquipClicked()
{
	if (TargetItemInstance)
	{
		APawn* OwningPawn = GetOwningPlayerPawn();
		if (!OwningPawn)
		{
			return;
		}

		if (UF4EquipmentComponent* EquipComp = OwningPawn->FindComponentByClass<UF4EquipmentComponent>())
		{
			EquipComp->EquipWeapon(TargetItemInstance);
		}
	}
	RemoveFromParent();
}

void UF4InventoryContextMenu::OnDropClicked()
{
	if (TargetItemInstance)
	{
		APawn* OwningPawn = GetOwningPlayerPawn();
		if (!OwningPawn)
		{
			return;
		}

		if (UF4InventoryComponent* InventoryComponent = OwningPawn->FindComponentByClass<UF4InventoryComponent>())
		{
			// TODO: 월드에 아이템 스폰 (Drop) 로직 추가
			InventoryComponent->RemoveItem(TargetItemInstance);
		}

	}
	RemoveFromParent();
}

void UF4InventoryContextMenu::OnQuickSlotClicked()
{
	if (!TargetItemInstance)
	{
		return;
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	if (UF4QuickSlotComponent* QuickSlotComp = OwningPawn->FindComponentByClass<UF4QuickSlotComponent>())
	{
		int32 QuickSlotEmptyIndex = QuickSlotComp->GetEmptyConsumableSlotIndex();
		if (QuickSlotEmptyIndex != -1)
		{
			QuickSlotComp->RegisterItem(QuickSlotEmptyIndex, TargetItemInstance);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("등록 가능한 빈 퀵슬롯이 없습니다!"));
		}
	}
	RemoveFromParent();
}

void UF4InventoryContextMenu::OnUnregisterQuickSlotClicked()
{
	if (!TargetItemInstance)
	{
		return;
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	if (UF4QuickSlotComponent* QuickSlotComp = OwningPawn->FindComponentByClass<UF4QuickSlotComponent>())
	{
		int32 QuickSlotIdx = QuickSlotComp->FindItemSlotIndex(TargetItemInstance);
		if (QuickSlotIdx != -1)
		{
			QuickSlotComp->UnregisterItem(QuickSlotIdx);
		}
	}
	RemoveFromParent();
}
