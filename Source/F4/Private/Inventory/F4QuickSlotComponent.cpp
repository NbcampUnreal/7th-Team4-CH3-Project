#include "Inventory/F4QuickSlotComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment.h"
#include "Inventory/F4ItemInstance.h"

UF4QuickSlotComponent::UF4QuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	QuickSlots.Init(nullptr, TotalSlotCount);
}

void UF4QuickSlotComponent::RegisterItem(int32 SlotIndex, UF4ItemInstance* ItemToRegister)
{
	if (!QuickSlots.IsValidIndex(SlotIndex) || QuickSlots[SlotIndex] == ItemToRegister)
	{
		return;
	}

	if (QuickSlots[SlotIndex] != nullptr)
	{
		ClearSlot(SlotIndex);
	}

	QuickSlots[SlotIndex] = ItemToRegister;

	UAbilitySystemComponent* ASC = GetOwnerASC();
	for (UF4ItemFragment* Fragment : ItemToRegister->ItemDefinition->Fragments)
	{
		if (!Fragment)
		{
			continue;
		}
		Fragment->OnItemAddedToQuickSlot(ASC, ItemToRegister, SlotIndex);
	}

	if (OnQuickSlotUpdated.IsBound())
	{
		OnQuickSlotUpdated.Broadcast(SlotIndex, ItemToRegister);
	}
}

void UF4QuickSlotComponent::ClearSlot(int32 SlotIndex)
{
	if (!IsRegisteredSlot(SlotIndex))
	{
		return;
	}

	UF4ItemInstance* Item = QuickSlots[SlotIndex];
	UAbilitySystemComponent* ASC = GetOwnerASC();
	for (UF4ItemFragment* Fragment : Item->ItemDefinition->Fragments)
	{
		if (!Fragment)
		{
			continue;
		}
		Fragment->OnItemRemovedFromQuickSlot(ASC, Item, SlotIndex);
	}

	QuickSlots[SlotIndex] = nullptr;
	OnQuickSlotUpdated.Broadcast(SlotIndex, nullptr);
}

void UF4QuickSlotComponent::UseSlot(int32 SlotIndex)
{
	UF4ItemInstance* Item = GetItemAtIndex(SlotIndex);
	if (!Item || !Item->ItemDefinition || Item->Quantity <= 0)
	{
		return;
	}

	FGameplayTag UsageTag;
	for (UF4ItemFragment* Fragment : Item->ItemDefinition->Fragments)
	{
		if (!Fragment)
		{
			continue;
		}
		UsageTag = Fragment->GetUsageEventTag();
		if (UsageTag.IsValid())
		{
			break;
		}
	}

	if (!UsageTag.IsValid())
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = GetOwnerASC())
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetOwner();
		Payload.OptionalObject = Item;
		Payload.EventTag = UsageTag;
		Payload.EventMagnitude = static_cast<float>(SlotIndex);

		ASC->HandleGameplayEvent(UsageTag, &Payload);
	}
}

void UF4QuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		if (UF4InventoryComponent* InventoryComp = Owner->FindComponentByClass<UF4InventoryComponent>())
		{
			InventoryComp->OnItemQuantityChanged.AddDynamic(this, &ThisClass::OnInventoryItemQuantityChanged);
			InventoryComp->OnItemRemoved.AddDynamic(this, &ThisClass::OnInventoryItemRemoved);
		}
	}
}

bool UF4QuickSlotComponent::IsRegisteredSlot(int32 SlotIndex) const
{
	return QuickSlots.IsValidIndex(SlotIndex) && QuickSlots[SlotIndex] != nullptr;
}

UAbilitySystemComponent* UF4QuickSlotComponent::GetOwnerASC() const
{
	if (IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		return ASInterface->GetAbilitySystemComponent();
	}
	return nullptr;
}

int32 UF4QuickSlotComponent::GetEmptyConsumableSlotIndex() const
{
	for (int32 i = 0; i < QuickSlots.Num(); ++i)
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

void UF4QuickSlotComponent::OnInventoryItemQuantityChanged(UF4ItemInstance* Item, int32 NewQuantity)
{
	const int32 SlotIndex = FindItemSlotIndex(Item);
	if (SlotIndex == -1)
	{
		return;
	}

	if (NewQuantity <= 0)
	{
		ClearSlot(SlotIndex);
	}
	else
	{
		OnQuickSlotUpdated.Broadcast(SlotIndex, Item);
	}
}

void UF4QuickSlotComponent::OnInventoryItemRemoved(UF4ItemInstance* RemovedItem)
{
	const int32 SlotIndex = FindItemSlotIndex(RemovedItem);
	if (SlotIndex == -1)
	{
		return;
	}

	ClearSlot(SlotIndex);
}
