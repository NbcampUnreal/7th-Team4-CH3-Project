#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/F4ItemFragment.h"
#include "F4ItemFragment_Usable.generated.h"

/**
 * 
 */
UCLASS()
class F4_API UF4ItemFragment_Usable : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	FGameplayTag UsageEventTag;
};
