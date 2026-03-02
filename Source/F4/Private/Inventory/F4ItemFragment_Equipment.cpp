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
			Instance->AddGrantedAbilityHandle(ASC->GiveAbility(Spec));
		}
	}
}

void UF4ItemFragment_Equipment::OnItemUnequipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance)
{
	if (!ASC || !Instance)
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : Instance->GetGrantedAbilityHandles())
	{
		ASC->ClearAbility(Handle);
	}
	Instance->ResetGrantedAbilityHandles();
}
