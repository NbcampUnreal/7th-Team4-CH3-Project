#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F4EquipmentComponent.generated.h"

class UF4ItemFragment_Equipment;
class UF4ItemInstance;
class UF4InventoryComponent;
class UAbilitySystemComponent;
class AF4WeaponActor;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary = 0,
	Secondary = 1,
	None = 255
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveWeaponChanged, UF4ItemInstance*, NewItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquippedToSlot, int32, QuickSlotIndex, UF4ItemInstance*, ItemInstance);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class F4_API UF4EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UF4EquipmentComponent();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItemToSlot(UF4ItemInstance* ItemToEquip, EWeaponSlot TargetSlot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipItemFromSlot(EWeaponSlot TargetSlot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void SetActiveWeapon(EWeaponSlot NewSlot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipWeapon(UF4ItemInstance* ItemToEquip);

	UPROPERTY(BlueprintAssignable, Category = "Equipment | Event")
	FOnActiveWeaponChanged OnActiveWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "Equipment | Event")
	FOnWeaponEquippedToSlot OnWeaponEquippedToSlot;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UF4ItemInstance* GetActiveWeaponInstance() const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	AF4WeaponActor* GetActiveWeaponActor() const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UF4ItemInstance* GetWeaponInSlot(EWeaponSlot Slot) const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	EWeaponSlot FindEquippedSlot(UF4ItemInstance* Item) const;

	EWeaponSlot GetActiveSlot() const { return ActiveSlot; }

	UFUNCTION()
	void OnInventoryItemRemoved(UF4ItemInstance* RemovedItem);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	EWeaponSlot ActiveSlot = EWeaponSlot::None;

	UPROPERTY()
	TMap<EWeaponSlot, UF4ItemInstance*> WeaponLoadout;

	UPROPERTY()
	TMap<UF4ItemInstance*, AF4WeaponActor*> SpawnedWeapons;

	FName GetHolsterSocketName(EWeaponSlot Slot) const;
	void CleanUpOldWeapon(UF4ItemInstance* OldItem, EWeaponSlot OldSlot);
	void SetupNewWeapon(UF4ItemInstance* NewItem);
	EWeaponSlot DetermineTargetSlotForNewWeapon() const;
};

