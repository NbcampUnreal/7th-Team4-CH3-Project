#include "Inventory/F4QuickSlotComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment.h"
#include "Inventory/F4ItemFragment_Usable.h"
#include "Inventory/F4ItemInstance.h"

UF4QuickSlotComponent::UF4QuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	QuickSlots.Init(nullptr, 8);
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

	if (!IsWeaponSlot(SlotIndex))
	{
		UAbilitySystemComponent* ASC = GetOwnerASC();
		for (UF4ItemFragment* Fragment : ItemToRegister->ItemDefinition->Fragments)
		{
			Fragment->OnItemAddedToQuickSlot(ASC, ItemToRegister, SlotIndex);
		}
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

	if (IsWeaponSlot(SlotIndex))
	{
		UnequipWeaponFromSlot(SlotIndex);
	}
	else
	{
		UF4ItemInstance* Item = QuickSlots[SlotIndex];
		UAbilitySystemComponent* ASC = GetOwnerASC();
		for (UF4ItemFragment* Fragment : Item->ItemDefinition->Fragments)
		{
			Fragment->OnItemRemovedFromQuickSlot(ASC, Item, SlotIndex);
		}
	}

	QuickSlots[SlotIndex] = nullptr;
	OnQuickSlotUpdated.Broadcast(SlotIndex, nullptr);
}

void UF4QuickSlotComponent::UseSlot(int32 SlotIndex)
{
	UF4ItemInstance* Item = GetItemAtIndex(SlotIndex);
	if (!Item || !Item->ItemDefinition)
	{
		return;
	}


	if (const UF4ItemFragment_Usable* UsableFragment = Item->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Usable>())
	{
		IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(GetOwner());
		if (!ASInterface)
		{
			return;
		}

		if (UAbilitySystemComponent* ASC = GetOwnerASC())
		{
			FGameplayEventData Payload;
			Payload.Instigator = GetOwner();
			Payload.OptionalObject = Item;
			Payload.EventTag = UsableFragment->UsageEventTag;
			Payload.EventMagnitude = static_cast<float>(SlotIndex);

			ASC->HandleGameplayEvent(UsableFragment->UsageEventTag, &Payload);
		}
	}
}

void UF4QuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		EquipmentComp = Owner->FindComponentByClass<UF4EquipmentComponent>();

		if (UF4InventoryComponent* InvComp = Owner->FindComponentByClass<UF4InventoryComponent>())
		{
			InvComp->OnItemQuantityChanged.AddDynamic(this, &ThisClass::OnInventoryItemQuantityChanged);
			InvComp->OnItemRemoved.AddDynamic(this, &ThisClass::OnInventoryItemRemoved);
		}

		if (EquipmentComp)
		{
			EquipmentComp->OnWeaponEquippedToSlot.AddDynamic(this, &ThisClass::OnWeaponEquippedToQuickSlot);
		}
	}
}

bool UF4QuickSlotComponent::IsRegisteredSlot(int32 SlotIndex) const
{
	return QuickSlots.IsValidIndex(SlotIndex) && QuickSlots[SlotIndex] != nullptr;
}

bool UF4QuickSlotComponent::IsWeaponSlot(int32 SlotIndex) const
{
	return SlotIndex == 0 || SlotIndex == 1;
}

UAbilitySystemComponent* UF4QuickSlotComponent::GetOwnerASC() const
{
	if (IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		return ASInterface->GetAbilitySystemComponent();
	}
	return nullptr;
}

void UF4QuickSlotComponent::UnequipWeaponFromSlot(int32 SlotIndex)
{
	if (EquipmentComp)
	{
		EWeaponSlot TargetSlot = (SlotIndex == 0) ? EWeaponSlot::Primary : EWeaponSlot::Secondary;
		EquipmentComp->UnequipItemFromSlot(TargetSlot);
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

void UF4QuickSlotComponent::OnInventoryItemQuantityChanged(UF4ItemInstance* Item, int32 NewQuantity)
{
	const int32 SlotIndex = FindItemSlotIndex(Item);
	if (SlotIndex == -1) return;

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
	if (SlotIndex != -1)
	{
		ClearSlot(SlotIndex);
	}
}

void UF4QuickSlotComponent::OnWeaponEquippedToQuickSlot(int32 QuickSlotIndex, UF4ItemInstance* Item)
{
	if (GetItemAtIndex(QuickSlotIndex) != Item)
	{
		RegisterItem(QuickSlotIndex, Item);
	}
}
