#pragma once

#include "CoreMinimal.h"
#include "Inventory/F4ItemFragment.h"
#include "F4ItemFragment_Firearm.generated.h"

class UF4ItemDefinition;

UCLASS()
class F4_API UF4ItemFragment_Firearm : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	TObjectPtr<UF4ItemDefinition> AmmoItemDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	float AmmoCostPerFire = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	float MaxMagazineSize = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float BaseDamage = 20.0f;
};
