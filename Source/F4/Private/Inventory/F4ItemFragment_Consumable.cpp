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
	if (!ASC || !Instance || !Instance->QuickSlotAbilityHandle.IsValid())
	{
		return;
	}

	if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Instance->QuickSlotAbilityHandle))
	{
		if (Spec->IsActive())
		{
			// 실행 중인 어빌리티는 강제 종료 대신 자연 종료 후 자동 제거
			Spec->RemoveAfterActivation = true;
		}
		else
		{
			ASC->ClearAbility(Instance->QuickSlotAbilityHandle);
		}
	}

	Instance->QuickSlotAbilityHandle = FGameplayAbilitySpecHandle();
}
