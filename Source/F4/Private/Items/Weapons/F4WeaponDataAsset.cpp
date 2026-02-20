#include "Items/Weapons/F4WeaponDataAsset.h"

FPrimaryAssetId UF4WeaponDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Weapon", GetFName());
}
