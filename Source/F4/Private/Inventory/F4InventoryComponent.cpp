#include "Inventory/F4InventoryComponent.h"

UF4InventoryComponent::UF4InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UF4InventoryComponent::AddItem(UF4ItemInstance* NewItem)
{
	if (NewItem)
	{
		InventoryList.Add(NewItem);
		OnInventoryUpdated.Broadcast();
	}
}

void UF4InventoryComponent::RemoveItem(UF4ItemInstance* ItemToRemove)
{
	if (ItemToRemove && InventoryList.Contains(ItemToRemove))
	{
		InventoryList.Remove(ItemToRemove);
		OnInventoryUpdated.Broadcast();
	}
}
