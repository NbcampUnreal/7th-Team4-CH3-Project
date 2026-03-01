#pragma once

#include "CoreMinimal.h"
#include "F4ItemFragment.h"
#include "F4ItemFragment_Spawnable.generated.h"

class AF4PickupActor;

UCLASS()
class F4_API UF4ItemFragment_Spawnable : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Class")
	TSubclassOf<AF4PickupActor> PickupActorClass;	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")

	int32 MinSpawnQuantity = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	int32 MaxSpawnQuantity = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	int32 QuantityMultiplier = 1;
};
