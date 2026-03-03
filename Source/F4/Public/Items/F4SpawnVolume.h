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
struct FWeightedTableEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> Table;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float TableWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 Count = 1;
};

USTRUCT(BlueprintType)
struct FSpawnTableGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeightedTableEntry> Tables;
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

	void RollAndCollectPaths(TArray<FSoftObjectPath>& OutPaths);

	void SpawnPreloaded();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Tables")
	TArray<FSpawnTableGroup> SpawnGroups;

private:
	const FWeightedTableEntry* SelectTableFromGroup(const FSpawnTableGroup& Group) const;
	TArray<TSoftObjectPtr<UF4ItemDefinition>> RollItemsFromTable(UDataTable* Table, int32 Count);

	bool GetRandomGroundPoint(FVector& OutLocation);

	void OnItemsLoaded(TArray<TSoftObjectPtr<UF4ItemDefinition>> RolledItems);
	void TrySpawnItem(UF4ItemDefinition* ItemDefinition);

	// SpawnManager 배치 로드용: 추첨 결과를 임시 보관
	TArray<TSoftObjectPtr<UF4ItemDefinition>> PrerolledItems;
};
