#pragma once

#include "CoreMinimal.h"
#include "F4ItemFragment.h"
#include "F4ItemFragment_Equipment.generated.h"

class UF4ItemInstance;
class UAnimInstance;
class AF4WeaponActor;
class UGameplayAbility;

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
	FName HandSocketName = TEXT("WeaponSocket");;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName HolsterSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	virtual void OnItemEquipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) override;
	virtual void OnItemUnequipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance) override;
	virtual TSubclassOf<UAnimInstance> GetEquipAnimLayer() const override { return AnimLayerClass; }
};
