#include "Inventory/F4QuickSlotComponent.h"

#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Consumable.h"
#include "Inventory/F4ItemFragment_Equipment.h"
#include "Inventory/F4ItemInstance.h"

UF4QuickSlotComponent::UF4QuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	QuickSlots.Init(nullptr, 8);
}

void UF4QuickSlotComponent::RegisterItem(int32 SlotIndex, UF4ItemInstance* ItemToRegister)
{
	if (!QuickSlots.IsValidIndex(SlotIndex) || !ItemToRegister || !ItemToRegister->ItemDefinition)
	{
		return;
	}

	if (SlotIndex == 0 || SlotIndex == 1)
	{
		if (!ItemToRegister->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Equipment>())
		{
			return;
		}

		EWeaponSlot TargetSlot = (SlotIndex == 0) ? EWeaponSlot::Primary : EWeaponSlot::Secondary;
		if (EquipmentComp)
		{
			EquipmentComp->EquipItemToSlot(ItemToRegister, TargetSlot);
		}
	}
	else
	{
		if (!ItemToRegister->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Consumable>())
		{
			return;
		}
	}

	QuickSlots[SlotIndex] = ItemToRegister;
	OnQuickSlotUpdated.Broadcast(SlotIndex, ItemToRegister);
}

void UF4QuickSlotComponent::UnregisterItem(int32 SlotIndex)
{
	if (QuickSlots.IsValidIndex(SlotIndex))
	{
		QuickSlots[SlotIndex] = nullptr;
		OnQuickSlotUpdated.Broadcast(SlotIndex, nullptr);
	}
}

void UF4QuickSlotComponent::ClearSlot(int32 SlotIndex)
{
	if (!QuickSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	if (SlotIndex == 0 || SlotIndex == 1)
	{
		EWeaponSlot TargetSlot = (SlotIndex == 0) ? EWeaponSlot::Primary : EWeaponSlot::Secondary;
		if (EquipmentComp)
		{
			EquipmentComp->UnequipItemFromSlot(TargetSlot);
		}
	}

	QuickSlots[SlotIndex] = nullptr;
	OnQuickSlotUpdated.Broadcast(SlotIndex, nullptr);
}

void UF4QuickSlotComponent::UseSlot(int32 SlotIndex)
{
	if (!QuickSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	UF4ItemInstance* TargetItem = QuickSlots[SlotIndex];
	if (!TargetItem)
	{
		return;
	}

	if (SlotIndex == 0 || SlotIndex == 1)
	{
		EWeaponSlot TargetSlot = (SlotIndex == 0) ? EWeaponSlot::Primary : EWeaponSlot::Secondary;

		if (EquipmentComp)
		{
			EquipmentComp->SetActiveWeapon(TargetSlot);
		}
	}
	else
	{
		// TODO: item 사용
		UE_LOG(LogTemp, Log, TEXT("%d번 슬롯 소모품 사용"), SlotIndex + 1);
	}
}

void UF4QuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		EquipmentComp = Owner->FindComponentByClass<UF4EquipmentComponent>();
		InventoryComp = Owner->FindComponentByClass<UF4InventoryComponent>();
	}
}

int32 UF4QuickSlotComponent::GetEmptyConsumableSlotIndex() const
{
	for (int32 i = 2; i < QuickSlots.Num(); ++i)
	{
		if (QuickSlots[i] == nullptr)
		{
			return i;
		}
	}
	return -1;
}

int32 UF4QuickSlotComponent::FindItemSlotIndex(UF4ItemInstance* ItemToFind) const
{
	if (!ItemToFind)
	{
		return -1;
	}

	for (int32 i = 0; i < QuickSlots.Num(); ++i)
	{
		if (QuickSlots[i] == ItemToFind)
		{
			return i;
		}
	}
	return -1;
}

UF4ItemInstance* UF4QuickSlotComponent::GetItemAtIndex(int32 Index) const
{
	if (!QuickSlots.IsValidIndex(Index))
	{
		return nullptr;
	}

	return QuickSlots[Index];
}
