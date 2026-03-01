#include "Inventory/F4EquipmentComponent.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment.h"
#include "Inventory/F4ItemFragment_Equipment.h"
#include "Inventory/F4ItemInstance.h"
#include "Items/Weapons/F4WeaponActor.h"

UF4EquipmentComponent::UF4EquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UF4EquipmentComponent::EquipItemToSlot(UF4ItemInstance* ItemToEquip, EWeaponSlot TargetSlot)
{
	if (!ItemToEquip || !ItemToEquip->ItemDefinition || !CharacterMesh || TargetSlot == EWeaponSlot::None)
	{
		return;
	}

	OnWeaponEquippedToSlot.Broadcast(static_cast<int32>(TargetSlot), ItemToEquip);

	if (WeaponLoadout.Contains(TargetSlot))
	{
		UnequipItemFromSlot(TargetSlot);
	}

	const UF4ItemFragment_Equipment* EquipmentFragment = ItemToEquip->ItemDefinition->FindFragmentByClass< UF4ItemFragment_Equipment>();
	if (!EquipmentFragment || !EquipmentFragment->WeaponActorClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AF4WeaponActor* NewWeaponActor = GetWorld()->SpawnActor<AF4WeaponActor>(EquipmentFragment->WeaponActorClass, SpawnParams);

	if (NewWeaponActor)
	{
		NewWeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetHolsterSocketName(TargetSlot));

		WeaponLoadout.Add(TargetSlot, ItemToEquip);
		SpawnedWeapons.Add(ItemToEquip, NewWeaponActor);

		if (ActiveSlot == EWeaponSlot::None)
		{
			SetActiveWeapon(TargetSlot);
		}
	}
}

void UF4EquipmentComponent::UnequipItemFromSlot(EWeaponSlot TargetSlot)
{
	if (TargetSlot == EWeaponSlot::None || !WeaponLoadout.Contains(TargetSlot))
	{
		return;
	}

	if (ActiveSlot == TargetSlot)
	{
		SetActiveWeapon(EWeaponSlot::None);
	}

	UF4ItemInstance* ItemToRemove = WeaponLoadout[TargetSlot];
	if (!ItemToRemove)
	{
		return;
	}

	AF4WeaponActor* WeaponActorToDestroy = SpawnedWeapons.FindRef(ItemToRemove);
	if (WeaponActorToDestroy)
	{
		WeaponActorToDestroy->Destroy();
	}

	SpawnedWeapons.Remove(ItemToRemove);
	WeaponLoadout.Remove(TargetSlot);
}

void UF4EquipmentComponent::SetActiveWeapon(EWeaponSlot NewSlot)
{
	if (ActiveSlot == NewSlot)
	{
		return;
	}

	if (ActiveSlot != EWeaponSlot::None && WeaponLoadout.Contains(ActiveSlot))
	{
		CleanUpOldWeapon(WeaponLoadout[ActiveSlot], ActiveSlot);
	}

	ActiveSlot = NewSlot;

	if (NewSlot == EWeaponSlot::None || !WeaponLoadout.Contains(NewSlot))
	{
		OnActiveWeaponChanged.Broadcast(nullptr);
		return;
	}

	UF4ItemInstance* NewItem = WeaponLoadout[NewSlot];
	SetupNewWeapon(NewItem);

	OnActiveWeaponChanged.Broadcast(NewItem);
}

void UF4EquipmentComponent::EquipWeapon(UF4ItemInstance* ItemToEquip)
{
	if (!ItemToEquip)
	{
		return;
	}

	for (const auto& Pair : WeaponLoadout)
	{
		if (Pair.Value == ItemToEquip)
		{
			SetActiveWeapon(Pair.Key);
			return;
		}
	}

	EWeaponSlot TargetSlot = DetermineTargetSlotForNewWeapon();

	EquipItemToSlot(ItemToEquip, TargetSlot);
	SetActiveWeapon(TargetSlot);
}

UF4ItemInstance* UF4EquipmentComponent::GetActiveWeaponInstance() const
{
	return WeaponLoadout.Contains(ActiveSlot) ? WeaponLoadout[ActiveSlot] : nullptr;
}

AF4WeaponActor* UF4EquipmentComponent::GetActiveWeaponActor() const
{
	if (ActiveSlot != EWeaponSlot::None && WeaponLoadout.Contains(ActiveSlot))
	{
		UF4ItemInstance* ActiveItem = WeaponLoadout[ActiveSlot];
		if (SpawnedWeapons.Contains(ActiveItem))
		{
			return SpawnedWeapons[ActiveItem];
		}
	}
	return nullptr;
}

void UF4EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
	{
		ASC = OwnerCharacter->FindComponentByClass<UAbilitySystemComponent>();
		CharacterMesh = OwnerCharacter->GetMesh();
	}
}

FName UF4EquipmentComponent::GetHolsterSocketName(EWeaponSlot Slot) const
{
	switch (Slot)
	{
	case EWeaponSlot::Primary:
		return TEXT("Holster_Primary");
	case EWeaponSlot::Secondary:
		return TEXT("Holster_Secondary");
	default:
		return NAME_None;
	}
}

void UF4EquipmentComponent::CleanUpOldWeapon(UF4ItemInstance* OldItem, EWeaponSlot OldSlot)
{
	if (!OldItem)
	{
		return;
	}

	AF4WeaponActor* OldWeaponActor = SpawnedWeapons.FindRef(OldItem);
	const UF4ItemFragment_Equipment* OldFragment = OldItem->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Equipment>();

	if (OldWeaponActor && OldFragment)
	{
		OldWeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetHolsterSocketName(OldSlot));
	}

	if (ASC)
	{
		for (UF4ItemFragment* Fragment : OldItem->ItemDefinition->Fragments)
		{
			Fragment->OnItemUnequipped(ASC, OldItem);
		}
	}
}

void UF4EquipmentComponent::SetupNewWeapon(UF4ItemInstance* NewItem)
{
	if (!NewItem || !ASC)
	{
		return;
	}

	AF4WeaponActor* NewWeaponActor = SpawnedWeapons.FindRef(NewItem);
	const UF4ItemFragment_Equipment* NewFragment = NewItem->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Equipment>();

	if (!NewWeaponActor || !NewFragment)
	{
		return;
	}

	NewWeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NewFragment->HandSocketName);

	for (UF4ItemFragment* Fragment : NewItem->ItemDefinition->Fragments)
	{
		Fragment->OnItemEquipped(ASC, NewItem);
	}
}

EWeaponSlot UF4EquipmentComponent::DetermineTargetSlotForNewWeapon() const
{
	if (!WeaponLoadout.Contains(EWeaponSlot::Primary))
	{
		return EWeaponSlot::Primary;
	}
	if (!WeaponLoadout.Contains(EWeaponSlot::Secondary))
	{
		return EWeaponSlot::Secondary;
	}
	return ActiveSlot;
}
