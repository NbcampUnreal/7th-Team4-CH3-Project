#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "F4ItemFragment.generated.h"

class UAbilitySystemComponent;
class UF4ItemInstance;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class F4_API UF4ItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnItemEquipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) {}
	virtual void OnItemUnequipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) {}
	virtual void OnItemAddedToQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex) {}
	virtual void OnItemRemovedFromQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex) {}
};
