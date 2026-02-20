#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "F4AbilitySet.generated.h"

UCLASS()
class F4_API UF4AbilitySet : public UDataAsset
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities; 
};
