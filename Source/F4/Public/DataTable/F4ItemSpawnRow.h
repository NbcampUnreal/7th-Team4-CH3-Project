#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "F4ItemSpawnRow.generated.h"

class UF4ItemDefinition;

USTRUCT(BlueprintType)
struct FF4ItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSoftObjectPtr<UF4ItemDefinition> ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0.0"))
	float SpawnWeight = 1.0f;
};
