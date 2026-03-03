#include "Items/F4SpawnManager.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Items/F4SpawnVolume.h"

AF4SpawnManager::AF4SpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AF4SpawnManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimerForNextTick(this, &AF4SpawnManager::StartBatchLoad);
}

void AF4SpawnManager::RegisterVolume(AF4SpawnVolume* Volume)
{
	if (Volume)
	{
		PendingVolumes.AddUnique(Volume);
	}
}

void AF4SpawnManager::StartBatchLoad()
{
	TArray<FSoftObjectPath> AllPaths;

	for (TWeakObjectPtr<AF4SpawnVolume>& WeakVol : PendingVolumes)
	{
		if (AF4SpawnVolume* Vol = WeakVol.Get())
		{
			Vol->RollAndCollectPaths(AllPaths);
		}
	}

	if (AllPaths.IsEmpty())
	{
		PendingVolumes.Empty();
		return;
	}

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		AllPaths,
		FStreamableDelegate::CreateUObject(this, &AF4SpawnManager::OnAllItemsLoaded)
	);
}

void AF4SpawnManager::OnAllItemsLoaded()
{
	for (TWeakObjectPtr<AF4SpawnVolume>& WeakVol : PendingVolumes)
	{
		if (AF4SpawnVolume* Vol = WeakVol.Get())
		{
			Vol->SpawnPreloaded();
		}
	}

	PendingVolumes.Empty();
}
