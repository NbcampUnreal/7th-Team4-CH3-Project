#include "Inventory/F4QuickSlotComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Consumable.h"
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
		GrantConsumableAbility(SlotIndex, ItemToRegister);
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
		ClearConsumableAbility(SlotIndex);
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

	if (GrantedAbilityHandles.Num() < 8)
	{
		GrantedAbilityHandles.SetNum(8);
	}

	if (AActor* Owner = GetOwner())
	{
		EquipmentComp = Owner->FindComponentByClass<UF4EquipmentComponent>();
		InventoryComp = Owner->FindComponentByClass<UF4InventoryComponent>();
	}
}

void UF4QuickSlotComponent::GrantConsumableAbility(int32 SlotIndex, UF4ItemInstance* Item)
{
	if (!GrantedAbilityHandles.IsValidIndex(SlotIndex))
	{
		GrantedAbilityHandles.SetNum(SlotIndex + 1);
	}

	if (!Item || !Item->ItemDefinition)
	{
		return;
	}

	const UF4ItemFragment_Consumable* ConsumableFrag = Item->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Consumable>();
	if (!ConsumableFrag)
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = GetOwnerASC())
	{
		FGameplayAbilitySpec Spec(ConsumableFrag->ConsumeAbility, 1, INDEX_NONE, Item);
		GrantedAbilityHandles[SlotIndex] = ASC->GiveAbility(Spec);
	}
}

void UF4QuickSlotComponent::ClearConsumableAbility(int32 SlotIndex)
{
	if (!GrantedAbilityHandles.IsValidIndex(SlotIndex) || !GrantedAbilityHandles[SlotIndex].IsValid())
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = GetOwnerASC())
	{
		ASC->ClearAbility(GrantedAbilityHandles[SlotIndex]);
		GrantedAbilityHandles[SlotIndex] = FGameplayAbilitySpecHandle();
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
