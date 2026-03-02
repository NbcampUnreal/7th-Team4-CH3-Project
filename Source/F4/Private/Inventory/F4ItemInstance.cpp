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

void UF4ItemInstance::AddGrantedAbilityHandle(const FGameplayAbilitySpecHandle& Handle)
{
	GrantedAbilityHandles.Add(Handle);
}

const TArray<FGameplayAbilitySpecHandle>& UF4ItemInstance::GetGrantedAbilityHandles() const
{
	return GrantedAbilityHandles;
}

void UF4ItemInstance::ResetGrantedAbilityHandles()
{
	GrantedAbilityHandles.Reset();
}

void UF4ItemInstance::SetActiveGEHandle(FActiveGameplayEffectHandle Handle)
{
	ActiveGEHandle = Handle;
}

FActiveGameplayEffectHandle UF4ItemInstance::GetActiveGEHandle() const
{
	return ActiveGEHandle;
}

void UF4ItemInstance::InvalidateActiveGEHandle()
{
	ActiveGEHandle.Invalidate();
}

void UF4ItemInstance::SetQuickSlotAbilityHandle(const FGameplayAbilitySpecHandle& Handle)
{
	QuickSlotAbilityHandle = Handle;
}

FGameplayAbilitySpecHandle UF4ItemInstance::GetQuickSlotAbilityHandle() const
{
	return QuickSlotAbilityHandle;
}

void UF4ItemInstance::InvalidateQuickSlotAbilityHandle()
{
	QuickSlotAbilityHandle = FGameplayAbilitySpecHandle();
}
