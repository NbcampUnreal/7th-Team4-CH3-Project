#include "Inventory/F4ItemFragment_Consumable.h"

#include "AbilitySystemComponent.h"
#include "Inventory/F4ItemInstance.h"

void UF4ItemFragment_Consumable::OnItemAddedToQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex)
{
	if (!ASC || !Instance || !ConsumeAbility)
	{
		return;
	}

	FGameplayAbilitySpec Spec(ConsumeAbility, 1, INDEX_NONE, Instance);
	Instance->QuickSlotAbilityHandle = ASC->GiveAbility(Spec);
}

void UF4ItemFragment_Consumable::OnItemRemovedFromQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex)
{
	if (!ASC || !Instance)
	{
		return;
	}

	ASC->ClearAbility(Instance->QuickSlotAbilityHandle);
	Instance->QuickSlotAbilityHandle = FGameplayAbilitySpecHandle();
}
