#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F4InventoryComponent.generated.h"

class UF4ItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UF4ItemInstance*, RemovedItemInstance);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class F4_API UF4InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UF4InventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(UF4ItemInstance* NewItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(UF4ItemInstance* ItemToRemove);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ConsumeItem(UF4ItemInstance* ItemToConsume, int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemRemoved OnItemRemoved;

	const TArray<TObjectPtr<UF4ItemInstance>>& GetInventoryItems() const { return InventoryList; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<UF4ItemInstance>> InventoryList;
};
