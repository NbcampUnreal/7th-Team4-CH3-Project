#include "Items/F4DropItem.h"
#include "Engine/AssetManager.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Spawnable.h"
#include "Items/F4PickupActor.h"

AF4DropItem::AF4DropItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AF4DropItem::TryDropItem(AActor* Monster, float DropChance)
{
	if (!Monster || !Monster->GetWorld() || FMath::FRand() > DropChance)
	{
		return;
	}

	UAssetManager& AssetManager = UAssetManager::Get();
	if (!UAssetManager::IsInitialized())
	{
		return;
	}
	
	TArray<FPrimaryAssetId> AllItemIds;
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("ItemDefinition"), AllItemIds);
	if (AllItemIds.IsEmpty())
	{
		return;
	}
	
	FPrimaryAssetId SelectedId = AllItemIds[FMath::RandRange(0, AllItemIds.Num() - 1)];
	
	FVector Start = Monster->GetActorLocation();
	FVector End = Start + (FVector::DownVector * 500.0f);
	FVector FinalLocation = Start;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Monster);
	
	if (Monster->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		FinalLocation = Hit.ImpactPoint + FVector(0.f, 0.f, 10.f);
	}
	
	TWeakObjectPtr<UWorld> WorldPtr = Monster->GetWorld();
	
	FStreamableDelegate Delegate = FStreamableDelegate::CreateStatic(
		&AF4DropItem::OnItemLoaded, SelectedId, FinalLocation, WorldPtr
	);
	
	AssetManager.GetStreamableManager().RequestAsyncLoad(
		AssetManager.GetPrimaryAssetPath(SelectedId), 
		Delegate
	);
}

void AF4DropItem::OnItemLoaded(FPrimaryAssetId AssetId, FVector SpawnLocation, TWeakObjectPtr<UWorld> WorldPtr)
{
	UWorld* World = WorldPtr.Get();
	if (!World)
	{
		return;
	}

	UAssetManager& AssetManager = UAssetManager::Get();
	UF4ItemDefinition* ItemDef = Cast<UF4ItemDefinition>(AssetManager.GetPrimaryAssetObject(AssetId));

	if (ItemDef)
	{
		const UF4ItemFragment_Spawnable* SpawnableFrag = ItemDef->FindFragmentByClass<UF4ItemFragment_Spawnable>();
		if (!SpawnableFrag || !SpawnableFrag->PickupActorClass) 
		{
			return;
		}
		
		FRotator SpawnRot = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);
		
		AF4PickupActor* NewItem = World->SpawnActorDeferred<AF4PickupActor>(
			SpawnableFrag->PickupActorClass,
			FTransform(SpawnRot, SpawnLocation)
		);

		if (NewItem)
		{
			int32 Quantity = FMath::RandRange(SpawnableFrag->MinSpawnQuantity, SpawnableFrag->MaxSpawnQuantity) 
							* SpawnableFrag->QuantityMultiplier;

			NewItem->ItemQuantity = FMath::Max(1, Quantity);
			
			NewItem->InitializePickup(ItemDef);
			NewItem->FinishSpawning(FTransform(SpawnRot, SpawnLocation));
		}
	}
}




