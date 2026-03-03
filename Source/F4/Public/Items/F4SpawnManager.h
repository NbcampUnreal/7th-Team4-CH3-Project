#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F4SpawnManager.generated.h"

class AF4SpawnVolume;

UCLASS()
class F4_API AF4SpawnManager : public AActor
{
	GENERATED_BODY()

public:
	AF4SpawnManager();

protected:
	virtual void BeginPlay() override;

public:
	void RegisterVolume(AF4SpawnVolume* Volume);

private:
	void StartBatchLoad();
	void OnAllItemsLoaded();

	TArray<TWeakObjectPtr<AF4SpawnVolume>> PendingVolumes;
};
