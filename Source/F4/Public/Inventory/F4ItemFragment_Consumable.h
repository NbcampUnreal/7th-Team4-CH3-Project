#pragma once

#include "CoreMinimal.h"
#include "F4ItemFragment.h"
#include "F4ItemFragment_Consumable.generated.h"

class UGameplayAbility;

UCLASS()
class F4_API UF4ItemFragment_Consumable : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> ConsumeAbility;
};
