#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "DataTable/F4ItemSpawnRow.h"
#include "F4SpawnVolume.generated.h"

class UF4ItemDefinition;
class AF4PickupActor;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FSpawnTableEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> Table;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 Count = 1;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Tables")
	TArray<FSpawnTableEntry> SpawnTables;

private:
	TArray<TSoftObjectPtr<UF4ItemDefinition>> RollItemsFromTable(UDataTable* Table, int32 Count);
	
	bool GetRandomGroundPoint(FVector& OutLocation);

	void OnItemsLoaded(TArray<TSoftObjectPtr<UF4ItemDefinition>> RolledItems);
	void TrySpawnItem(UF4ItemDefinition* ItemDefinition);
};
