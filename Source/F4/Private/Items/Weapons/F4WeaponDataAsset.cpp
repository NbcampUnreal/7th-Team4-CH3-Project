#include "Items/Weapons/F4WeaponDataAsset.h"

UF4WeaponDataAsset::UF4WeaponDataAsset()
{
	ItemType = EF4ItemType::Weapon;
}

FPrimaryAssetId UF4WeaponDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Weapon", GetFName());
}
