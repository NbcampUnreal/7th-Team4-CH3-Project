#pragma once

#include "CoreMinimal.h"
#include "Inventory/F4ItemFragment.h"
#include "F4ItemFragment_Stackable.generated.h"

UCLASS()
class F4_API UF4ItemFragment_Stackable : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stacking")
	int32 MaxStackSize = 99;
};
