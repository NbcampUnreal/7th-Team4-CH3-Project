#include "Inventory/F4InventoryComponent.h"

#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Stackable.h"
#include "Inventory/F4ItemInstance.h"

UF4InventoryComponent::UF4InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UF4InventoryComponent::AddItem(UF4ItemInstance* NewItem)
{
	if (!NewItem || !NewItem->ItemDefinition)
	{
		return;
	}

	const UF4ItemFragment_Stackable* StackFragment = NewItem->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Stackable>();
	if (!StackFragment)
	{
		InventoryList.Add(NewItem);
		OnInventoryUpdated.Broadcast();
		return;
	}

	int32 QuantityLeft = NewItem->Quantity;
	const int32 MaxStack = StackFragment->MaxStackSize;

	for (UF4ItemInstance* ExistItem : InventoryList)
	{
		if (ExistItem->ItemDefinition == NewItem->ItemDefinition && ExistItem->Quantity < MaxStack)
		{
			int32 SpaceLeft = MaxStack - ExistItem->Quantity;
			int32 AmountToAdd = FMath::Min(QuantityLeft, SpaceLeft);

			ExistItem->Quantity += AmountToAdd;
			QuantityLeft -= AmountToAdd;

			if (QuantityLeft <= 0)
			{
				break;
			}
		}
	}

	if (QuantityLeft > 0)
	{
		NewItem->Quantity = QuantityLeft;
		InventoryList.Add(NewItem);
	}
	else
	{
		NewItem->MarkAsGarbage();
	}

	OnInventoryUpdated.Broadcast();
}

void UF4InventoryComponent::RemoveItem(UF4ItemInstance* ItemToRemove)
{
	if (ItemToRemove && InventoryList.Contains(ItemToRemove))
	{
		InventoryList.Remove(ItemToRemove);
		OnInventoryUpdated.Broadcast();
	}
}

void UF4InventoryComponent::ConsumeItem(UF4ItemInstance* ItemToConsume, int32 Amount)
{
	if (!ItemToConsume || !InventoryList.Contains(ItemToConsume))
	{
		return;
	}

	ItemToConsume->Quantity -= Amount;

	if (ItemToConsume->Quantity <= 0)
	{
		InventoryList.Remove(ItemToConsume);
		OnItemRemoved.Broadcast(ItemToConsume);

		ItemToConsume->MarkAsGarbage();
	}

	OnInventoryUpdated.Broadcast();
}
