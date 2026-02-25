#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/F4ItemFragment.h"
#include "Items/Weapons/F4WeaponActor.h"
#include "F4ItemFragment_Equipment.generated.h"

class UAnimInstance;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FWeaponAbilitySet
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag InputTag; 
};

UCLASS()
class F4_API UF4ItemFragment_Equipment : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<AF4WeaponActor> WeaponActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment | Animation")
	TSubclassOf<UAnimInstance> AnimLayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName HandSocketName = TEXT("hand_r");;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName HolsterSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Abilities")
	TArray<FWeaponAbilitySet> GrantedAbilities;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	// TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
