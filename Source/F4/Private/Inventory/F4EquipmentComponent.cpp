#include "Inventory/F4EquipmentComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetWeapon.h"
#include "GameFramework/Character.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Equipment.h"
#include "Inventory/F4ItemFragment_Firearm.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4QuickSlotComponent.h"
#include "Items/Weapons/F4WeaponActor.h"
#include "System/F4GameplayTags.h"

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

		if (UF4QuickSlotComponent* QuickSlotComp = GetOwner()->FindComponentByClass<UF4QuickSlotComponent>())
		{
			int32 QuickSlotIndex = static_cast<int32>(TargetSlot);
			if (QuickSlotComp->GetItemAtIndex(QuickSlotIndex) != ItemToEquip)
			{
				QuickSlotComp->RegisterItem(QuickSlotIndex, ItemToEquip);
			}
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

	UF4QuickSlotComponent* QuickSlotComp = GetOwner()->FindComponentByClass<UF4QuickSlotComponent>();
	if (QuickSlotComp)
	{
		QuickSlotComp->UnregisterItem(static_cast<int32>(TargetSlot));
	}
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
	if (TargetSlot == ActiveSlot && WeaponLoadout.Contains(TargetSlot))
	{
		UnequipItemFromSlot(TargetSlot);
	}

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

void UF4EquipmentComponent::ApplyWeaponStatEffect(UF4ItemInstance* ItemInstance, const UF4ItemFragment_Firearm* FirearmFragment)
{
	if (!FirearmFragment || !FirearmFragment->EquipStatEffect || !ASC)
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddInstigator(GetOwner(), GetOwner());
	ContextHandle.AddSourceObject(ItemInstance);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(FirearmFragment->EquipStatEffect, 1.0f, ContextHandle);

	if (SpecHandle.IsValid())
	{
		CurrentWeaponGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UF4EquipmentComponent::GrantWeaponAbilities(UF4ItemInstance* Item, const UF4ItemFragment_Equipment* Fragment)
{
	if (!Item || !Fragment || !ASC)
	{
		return;
	}

	FEquipmentHandles NewHandles;
	for (TSubclassOf<UGameplayAbility> AbilityClass : Fragment->GrantedAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, Item);
			NewHandles.AbilitySpecHandles.Add(ASC->GiveAbility(Spec));
		}
	}
	GrantedHandlesMap.Add(Item, NewHandles);
}

void UF4EquipmentComponent::RemoveWeaponAbilities(UF4ItemInstance* Item)
{
	if (!Item || !ASC || !GrantedHandlesMap.Contains(Item))
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : GrantedHandlesMap[Item].AbilitySpecHandles)
	{
		ASC->ClearAbility(Handle);
	}
	GrantedHandlesMap.Remove(Item);
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
		if (ASC->HasAttributeSetForAttribute(UF4AttributeSetWeapon::GetCurrentAmmoAttribute()))
		{
			float LeftAmmo = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetCurrentAmmoAttribute());
			OldItem->DynamicStats.Add(F4GameplayTags::Weapon_Ammo_Loaded, LeftAmmo);
		}

		RemoveWeaponAbilities(OldItem);
		if (CurrentWeaponGEHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(CurrentWeaponGEHandle);
			CurrentWeaponGEHandle.Invalidate();
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

	if (const UF4ItemFragment_Firearm* FirearmFragment = NewItem->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Firearm>())
	{
		ApplyWeaponStatEffect(NewItem, FirearmFragment);
	}

	float SavedAmmo = NewItem->GetStatValue(F4GameplayTags::Weapon_Ammo_Loaded);
	ASC->SetNumericAttributeBase(UF4AttributeSetWeapon::GetCurrentAmmoAttribute(), SavedAmmo);

	GrantWeaponAbilities(NewItem, NewFragment);
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
