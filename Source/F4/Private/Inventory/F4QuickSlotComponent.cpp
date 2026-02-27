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

	QuickSlots[SlotIndex] = ItemToRegister;
	GrantConsumableAbility(SlotIndex, ItemToRegister);

	if (OnQuickSlotUpdated.IsBound())
	{
		OnQuickSlotUpdated.Broadcast(SlotIndex, ItemToRegister);
	}
}

void UF4QuickSlotComponent::UnregisterItem(int32 SlotIndex)
{
	if (!QuickSlots.IsValidIndex(SlotIndex) || !QuickSlots[SlotIndex])
	{
		return;
	}

	ClearConsumableAbility(SlotIndex);

	QuickSlots[SlotIndex] = nullptr;
	OnQuickSlotUpdated.Broadcast(SlotIndex, nullptr);
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

		if (UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent())
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
	if (!ConsumableFrag) return;

	IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASInterface) return;

	UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent();
	if (!ASC) return;

	FGameplayAbilitySpec Spec(ConsumableFrag->ConsumeAbility, 1, INDEX_NONE, Item);
	GrantedAbilityHandles[SlotIndex] = ASC->GiveAbility(Spec);
}

void UF4QuickSlotComponent::ClearConsumableAbility(int32 SlotIndex)
{
	if (!GrantedAbilityHandles.IsValidIndex(SlotIndex) || !GrantedAbilityHandles[SlotIndex].IsValid()) return;

	IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASInterface) return;

	UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent();
	if (!ASC) return;

	ASC->ClearAbility(GrantedAbilityHandles[SlotIndex]);
	GrantedAbilityHandles[SlotIndex] = FGameplayAbilitySpecHandle();
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
