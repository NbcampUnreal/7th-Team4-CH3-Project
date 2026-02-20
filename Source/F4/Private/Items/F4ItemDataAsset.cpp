#include "Items/F4ItemDataAsset.h"

FPrimaryAssetId UF4ItemDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Item", GetFName());
}
