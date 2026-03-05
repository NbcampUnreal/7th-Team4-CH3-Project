#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDropData.generated.h"

USTRUCT(Blueprintable)
struct FItemDropData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> ItemClass;
};
