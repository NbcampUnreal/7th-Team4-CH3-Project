#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F4DropItem.generated.h"

UCLASS()
class F4_API AF4DropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AF4DropItem();
	
public:
	static void TryDropItem(AActor* Monster, float DropChance = 0.5f);

private:
	static void OnItemLoaded(FPrimaryAssetId AssetId, FVector SpawnLocation, TSoftObjectPtr<UWorld> WorldPtr);
};
