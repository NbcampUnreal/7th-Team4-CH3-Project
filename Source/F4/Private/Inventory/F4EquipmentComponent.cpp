#include "Inventory/F4EquipmentComponent.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Equipment.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4QuickSlotComponent.h"
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

	if (WeaponLoadout.Contains(TargetSlot))
	{
		UnequipItemFromSlot(TargetSlot);
	}

	const UF4ItemFragment_Equipment* EquipmentFragment = ItemToEquip->ItemDefinition->FindFragmentByClass<
		UF4ItemFragment_Equipment>();
	if (!EquipmentFragment || !EquipmentFragment->WeaponActorClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AF4WeaponActor* NewWeaponActor = GetWorld()->SpawnActor<AF4WeaponActor>(
		EquipmentFragment->WeaponActorClass, SpawnParams);

	if (NewWeaponActor)
	{
		NewWeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		                                  GetHolsterSocketName(TargetSlot));

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
	if (TargetSlot == EWeaponSlot::None || !WeaponLoadout.Contains(TargetSlot)) return;

	if (ActiveSlot == TargetSlot)
	{
		SetActiveWeapon(EWeaponSlot::None);
	}

	UF4ItemInstance* ItemToRemove = WeaponLoadout[TargetSlot];

	if (SpawnedWeapons.Contains(ItemToRemove))
	{
		AF4WeaponActor* WeaponActorToDestroy = SpawnedWeapons[ItemToRemove];
		if (WeaponActorToDestroy)
		{
			// TODO, dooyeon: (선택 사항) 파괴하기 전에 AttributeSet의 현재 탄약 수를 ItemToRemove->DynamicStats 에 덮어써서 백업하는 로직 추가 가능
			WeaponActorToDestroy->Destroy();
		}
		SpawnedWeapons.Remove(ItemToRemove);
	}

	WeaponLoadout.Remove(TargetSlot);
	if (UF4QuickSlotComponent* QuickSlotComp = GetOwner()->FindComponentByClass<UF4QuickSlotComponent>())
	{
		int32 QuickSlotIndex = static_cast<int32>(TargetSlot);
		QuickSlotComp->UnregisterItem(QuickSlotIndex);
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
		UF4ItemInstance* OldItem = WeaponLoadout[ActiveSlot];
		AF4WeaponActor* OldWeaponActor = SpawnedWeapons.Contains(OldItem) ? SpawnedWeapons[OldItem] : nullptr;
		const UF4ItemFragment_Equipment* OldFragment = OldItem->ItemDefinition->FindFragmentByClass<
			UF4ItemFragment_Equipment>();

		if (OldWeaponActor && OldFragment)
		{
			OldWeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                  GetHolsterSocketName(ActiveSlot));

			if (ASC && GrantedHandlesMap.Contains(OldItem))
			{
				for (FGameplayAbilitySpecHandle Handle : GrantedHandlesMap[OldItem].AbilitySpecHandles)
				{
					ASC->ClearAbility(Handle);
				}
				GrantedHandlesMap.Remove(OldItem);
			}
		}
	}

	UF4ItemInstance* NewItem = nullptr;
	if (NewSlot != EWeaponSlot::None && WeaponLoadout.Contains(NewSlot))
	{
		NewItem = WeaponLoadout[NewSlot];
		AF4WeaponActor* NewWeaponActor = SpawnedWeapons.Contains(NewItem) ? SpawnedWeapons[NewItem] : nullptr;
		const UF4ItemFragment_Equipment* NewFragment = NewItem->ItemDefinition->FindFragmentByClass< UF4ItemFragment_Equipment>();

		if (NewWeaponActor && NewFragment)
		{
			NewWeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                  NewFragment->HandSocketName);

			if (ASC)
			{
				FEquipmentHandles NewHandles;
				for (TSubclassOf<UGameplayAbility> AbilityClass : NewFragment->GrantedAbilities)
				{
					if (AbilityClass)
					{
						FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, NewItem);
						NewHandles.AbilitySpecHandles.Add(ASC->GiveAbility(Spec));
					}
				}
				GrantedHandlesMap.Add(NewItem, NewHandles);
			}
		}
	}

	ActiveSlot = NewSlot;
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

	EWeaponSlot TargetSlot = EWeaponSlot::None;
	if (!WeaponLoadout.Contains(EWeaponSlot::Primary))
	{
		TargetSlot = EWeaponSlot::Primary;
	}
	else if (!WeaponLoadout.Contains(EWeaponSlot::Secondary))
	{
		TargetSlot = EWeaponSlot::Secondary;
	}
	else
	{
		TargetSlot = ActiveSlot;
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
