#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F4SpawnVolume.generated.h"

class UF4ItemDefinition;
class AF4PickupActor;
class UBoxComponent;

UCLASS()
class F4_API AF4SpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AF4SpawnVolume();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnItemsAsync();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "1"))
	int32 SpawnCount = 1;

private:
	bool GetRandomGroundPoint(FVector& OutLocation);

	void OnItemsLoaded(TArray<FPrimaryAssetId> LoadedAssetIds);
	void TrySpawnItem(UF4ItemDefinition* ItemDefinition);
};
