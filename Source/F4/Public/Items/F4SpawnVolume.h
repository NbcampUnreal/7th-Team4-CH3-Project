#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F4SpawnVolume.generated.h"

class UF4ItemDefinition;
class AF4PickupActor;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FF4SpawnItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TObjectPtr<UF4ItemDefinition> ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "1"))
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AF4PickupActor> OverridePickupClass;
};

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
	void SpawnItems();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<FF4SpawnItemData> SpawnableItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AF4PickupActor> DefaultItemClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "1"))
	int32 SpawnCount = 1;

private:
	bool GetRandomGroundPoint(FVector& OutLocation);
};
