#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "F4EquipmentComponent.generated.h"

class UF4ItemInstance;
class UAbilitySystemComponent;
class AF4WeaponActor;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	None,
	Primary,
	Secondary
};

USTRUCT()
struct FEquipmentHandles
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

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

	TMap<UF4ItemInstance*, FEquipmentHandles> GrantedHandlesMap;
};
