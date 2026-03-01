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
	if (!NewItem || !NewItem->ItemDefinition) return;

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

			OnItemQuantityChanged.Broadcast(ExistItem, ExistItem->Quantity);

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

		OnItemQuantityChanged.Broadcast(NewItem, NewItem->Quantity);
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
		OnItemRemoved.Broadcast(ItemToRemove);
		OnInventoryUpdated.Broadcast();
	}
}

void UF4InventoryComponent::ConsumeItem(UF4ItemInstance* ItemToConsume, int32 Amount)
{
	if (!ItemToConsume || !InventoryList.Contains(ItemToConsume)) return;

	ItemToConsume->Quantity -= Amount;
	const bool bIsDepleted = (ItemToConsume->Quantity <= 0);

	if (!bIsDepleted)
	{
		OnItemQuantityChanged.Broadcast(ItemToConsume, ItemToConsume->Quantity);
	}

	if (bIsDepleted)
	{
		InventoryList.Remove(ItemToConsume);
		OnItemRemoved.Broadcast(ItemToConsume);

		ItemToConsume->MarkAsGarbage();
	}

	OnInventoryUpdated.Broadcast();
}

int32 UF4InventoryComponent::GetTotalItemCountByDefinition(class UF4ItemDefinition* ItemDef) const
{
	if (!ItemDef)
	{
		return 0;
	}


	int32 TotalCount = 0;
	for (UF4ItemInstance* Item : InventoryList)
	{
		if (Item && Item->ItemDefinition == ItemDef)
		{
			TotalCount += Item->Quantity;
		}
	}
	return TotalCount;
}

void UF4InventoryComponent::ConsumeItemByDefinition(class UF4ItemDefinition* ItemDef, int32 AmountToConsume)
{
	if (!ItemDef || AmountToConsume <= 0) return;

	int32 RemainingToConsume = AmountToConsume;

	for (int32 i = InventoryList.Num() - 1; i >= 0; --i)
	{
		UF4ItemInstance* CurrentItem = InventoryList[i];
		if (CurrentItem && CurrentItem->ItemDefinition == ItemDef)
		{
			int32 TakeAmount = FMath::Min(CurrentItem->Quantity, RemainingToConsume);
			CurrentItem->Quantity -= TakeAmount;
			RemainingToConsume -= TakeAmount;

			if (CurrentItem->Quantity <= 0)
			{
				InventoryList.RemoveAt(i);
			}

			if (RemainingToConsume <= 0) break;
		}
	}

	OnInventoryUpdated.Broadcast();
}

