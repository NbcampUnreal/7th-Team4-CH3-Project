#pragma once

#include "CoreMinimal.h"
#include "F4ItemFragment.h"
#include "System/F4GameplayTags.h"
#include "F4ItemFragment_Consumable.generated.h"

class UGameplayAbility;

UCLASS()
class F4_API UF4ItemFragment_Consumable : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> ConsumeAbility;

	virtual void OnItemAddedToQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex) override;
	virtual void OnItemRemovedFromQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex) override;

	virtual FGameplayTag GetUsageEventTag() const override { return F4GameplayTags::Event_Trigger_UseConsumable; }
};
