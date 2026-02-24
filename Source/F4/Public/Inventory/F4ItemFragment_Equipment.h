#pragma once

#include "CoreMinimal.h"
#include "Inventory/F4ItemFragment.h"
#include "Items/Weapons/F4WeaponActor.h"
#include "F4ItemFragment_Equipment.generated.h"

class UF4BaseAnimInst;
class UGameplayAbility;

UCLASS()
class F4_API UF4ItemFragment_Equipment : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<AF4WeaponActor> WeaponActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment | Animation")
	TSubclassOf<UF4BaseAnimInst> AnimLayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
