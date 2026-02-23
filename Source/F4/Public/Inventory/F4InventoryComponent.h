// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F4InventoryComponent.generated.h"

class UF4ItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

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

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	const TArray<UF4ItemInstance*>& GetInventoryItems() const { return InventoryList; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UF4ItemInstance>> InventoryList;
};
