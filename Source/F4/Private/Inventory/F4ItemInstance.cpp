#include "Inventory/F4ItemInstance.h"

UF4ItemInstance::UF4ItemInstance()
{
	Quantity = 1;
}

void UF4ItemInstance::AddStatValue(FGameplayTag StatTag, float Value)
{
	if (DynamicStats.Contains(StatTag))
	{
		DynamicStats[StatTag] += Value;
	}
	else
	{
		DynamicStats.Add(StatTag, Value);
	}
}

float UF4ItemInstance::GetStatValue(FGameplayTag StatTag) const
{
	if (const float* FoundValue = DynamicStats.Find(StatTag))
	{
		return *FoundValue;
	}

	return 0.0f;
}
