#pragma once

#include "CoreMinimal.h"
#include "Inventory/F4ItemFragment.h"
#include "F4ItemFragment_Firearm.generated.h"

class UGameplayEffect;
class UF4ItemDefinition;

UCLASS()
class F4_API UF4ItemFragment_Firearm : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	TObjectPtr<UF4ItemDefinition> AmmoItemDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Stats")
	TSubclassOf<UGameplayEffect> EquipStatEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	float AmmoCostPerFire = 1.0f;

	virtual void OnItemEquipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) override;
	virtual void OnItemUnequipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) override;
};
