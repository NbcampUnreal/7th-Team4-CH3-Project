// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/F4WeaponDataAsset.h"

FPrimaryAssetId UF4WeaponDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Weapon", GetFName());
}
