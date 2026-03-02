#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "F4SpawnVolume.generated.h"

class UF4ItemDefinition;
class AF4PickupActor;
class UBoxComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning|Tables")
	TObjectPtr<UDataTable> WeaponSpawnTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Tables", meta = (ClampMin = "0"))
	int32 WeaponSpawnCount = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning|Tables")
	TObjectPtr<UDataTable> PotionSpawnTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning|Tables", meta = (ClampMin = "0"))
	int32 PotionSpawnCount = 5;

private:
	TArray<TSoftObjectPtr<UF4ItemDefinition>> RollItemsFromTable(UDataTable* Table, int32 Count);
	
	bool GetRandomGroundPoint(FVector& OutLocation);

	void OnItemsLoaded(TArray<TSoftObjectPtr<UF4ItemDefinition>> RolledItems);
	void TrySpawnItem(UF4ItemDefinition* ItemDefinition);
};
