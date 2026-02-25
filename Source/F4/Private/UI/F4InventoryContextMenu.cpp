// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/F4InventoryContextMenu.h"

#include "Components/Button.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Consumable.h"
#include "Inventory/F4ItemFragment_Equipment.h"
#include "Inventory/F4ItemInstance.h"

void UF4InventoryContextMenu::InitMenu(UF4ItemInstance* InItemInstance)
{
	TargetItemInstance = InItemInstance;
	if (!TargetItemInstance || !TargetItemInstance->ItemDefinition) return;

	const bool bIsEquippable = TargetItemInstance->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Equipment>() != nullptr;
	const bool bIsConsumable = TargetItemInstance->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Consumable>() != nullptr;

	if (EquipButton)
	{
		EquipButton->SetVisibility(bIsEquippable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (QuickSlotButton)
	{
		QuickSlotButton->SetVisibility(bIsConsumable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
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
	if (TargetItemInstance)
	{
		// QuickSlotComponent->RegisterItem(TargetItemInstance);
		UE_LOG(LogTemp, Log, TEXT("QuickSlot Registered"));
	}
	RemoveFromParent();
}
