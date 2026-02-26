#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F4QuickSlotComponent.generated.h"

class UF4InventoryComponent;
class UF4EquipmentComponent;
class UF4ItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotUpdated, int32, SlotIndex, UF4ItemInstance*, ItemInstance);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class F4_API UF4QuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UF4QuickSlotComponent();

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

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UF4EquipmentComponent> EquipmentComp;

	UPROPERTY()
	TObjectPtr<UF4InventoryComponent> InventoryComp;
};
