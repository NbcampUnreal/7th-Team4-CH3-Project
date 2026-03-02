#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F4QuickSlotComponent.generated.h"

class UAbilitySystemComponent;
class UF4ItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotUpdated, int32, SlotIndex, UF4ItemInstance*, ItemInstance);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class F4_API UF4QuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UF4QuickSlotComponent();

	static constexpr int32 TotalSlotCount = 6;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuickSlot")
	TArray<UF4ItemInstance*> QuickSlots;

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void RegisterItem(int32 SlotIndex, UF4ItemInstance* ItemToRegister);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void ClearSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void UseSlot(int32 SlotIndex);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnQuickSlotUpdated OnQuickSlotUpdated;

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	int32 GetEmptyConsumableSlotIndex() const;

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	int32 FindItemSlotIndex(UF4ItemInstance* ItemToFind) const;

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	UF4ItemInstance* GetItemAtIndex(int32 Index) const;

	UFUNCTION()
	void OnInventoryItemQuantityChanged(UF4ItemInstance* Item, int32 NewQuantity);

	UFUNCTION()
	void OnInventoryItemRemoved(UF4ItemInstance* RemovedItem);

protected:
	virtual void BeginPlay() override;

private:
	bool IsRegisteredSlot(int32 SlotIndex) const;
	UAbilitySystemComponent* GetOwnerASC() const;
};
