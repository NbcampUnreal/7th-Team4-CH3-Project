#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "F4ItemFragment.generated.h"

class UAbilitySystemComponent;
class UAnimInstance;
class UF4ItemInstance;
class UStaticMeshComponent;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class F4_API UF4ItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnItemEquipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) {}
	virtual void OnItemUnequipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) {}
	virtual void OnItemAddedToQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex) {}
	virtual void OnItemRemovedFromQuickSlot(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance, int32 SlotIndex) {}

	virtual TSubclassOf<UAnimInstance> GetEquipAnimLayer() const { return nullptr; }
	virtual void OnPickupActorSpawned(UStaticMeshComponent* MeshComp, UStaticMeshComponent* SubMeshComp) {}

	// UseSlot 호출 시 전송할 이벤트 태그. 사용 가능한 Fragment만 유효한 태그를 반환
	virtual FGameplayTag GetUsageEventTag() const { return FGameplayTag(); }
};
