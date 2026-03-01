#include "Inventory/F4ItemFragment_Equipment.h"

#include "AbilitySystemComponent.h"
#include "Inventory/F4ItemInstance.h"

void UF4ItemFragment_Equipment::OnItemEquipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance)
{
	if (!ASC || !Instance)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility> AbilityClass : GrantedAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, Instance);
			Instance->GrantedAbilityHandles.Add(ASC->GiveAbility(Spec));
		}
	}
}

void UF4ItemFragment_Equipment::OnItemUnequipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance)
{
	if (!ASC || !Instance)
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : Instance->GrantedAbilityHandles)
	{
		ASC->ClearAbility(Handle);
	}
	Instance->GrantedAbilityHandles.Reset();
}
