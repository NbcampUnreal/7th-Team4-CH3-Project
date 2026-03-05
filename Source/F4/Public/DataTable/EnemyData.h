#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyData.generated.h"

USTRUCT(Blueprintable)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceRange = 0.f;
};
