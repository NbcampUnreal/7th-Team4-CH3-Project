#include "Items/F4SpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "DataTable/F4ItemSpawnRow.h"
#include "Items/F4PickupActor.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Spawnable.h"
#include "Kismet/KismetMathLibrary.h"

AF4SpawnVolume::AF4SpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SetRootComponent(SpawningBox);
	
	SpawningBox->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));
}

void AF4SpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawnItemsAsync();
}

void AF4SpawnVolume::SpawnItemsAsync()
{
	TArray<TSoftObjectPtr<UF4ItemDefinition>> ItemsToLoad;
	
	for (const FSpawnTableEntry& Entry : SpawnTables)
	{
		ItemsToLoad.Append(RollItemsFromTable(Entry.Table, Entry.Count));
	}
	
	if (ItemsToLoad.IsEmpty())
	{
		return;
	}
	
	TArray<FSoftObjectPath> PathsToLoad;
	for (const TSoftObjectPtr<UF4ItemDefinition> SoftPtr : ItemsToLoad)
	{
		PathsToLoad.AddUnique(SoftPtr.ToSoftObjectPath());
	}
	
	UAssetManager& AssetManager = UAssetManager::Get();
	FStreamableDelegate Delegate = 
		FStreamableDelegate::CreateUObject(this, &AF4SpawnVolume::OnItemsLoaded, ItemsToLoad);
	
	AssetManager.GetStreamableManager().RequestAsyncLoad(PathsToLoad, Delegate);
}

TArray<TSoftObjectPtr<UF4ItemDefinition>> AF4SpawnVolume::RollItemsFromTable(UDataTable* Table, int32 Count)
{
	TArray<TSoftObjectPtr<UF4ItemDefinition>> SelectedItems;
	if (!Table || Count <= 0)
	{
		return SelectedItems;
	}
	
	TArray<FF4ItemSpawnRow*> AllRows;
	Table->GetAllRows<FF4ItemSpawnRow>(TEXT("SpawnTableContext"), AllRows);
	
	if (AllRows.IsEmpty())
	{
		return SelectedItems;
	}
	
	float TotalWeight = 0.0f;
	for (FF4ItemSpawnRow* Row : AllRows)
	{
		if (Row && Row->SpawnWeight > 0.0f)
		{
			TotalWeight += Row->SpawnWeight;
		}
	}

	if (TotalWeight <= 0.0f)
	{
		return SelectedItems;
	}

	for (int32 i = 0; i < Count; ++i)
	{
		float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
		float CurrentWeight = 0.0f;
		
		for (FF4ItemSpawnRow* Row : AllRows)
		{
			if (!Row || Row->SpawnWeight <= 0.0f)
			{
				continue;
			}
			
			CurrentWeight += Row->SpawnWeight;
			if (RandomValue <= CurrentWeight)
			{
				if (!Row->ItemDefinition.IsNull())
				{
					SelectedItems.Add(Row->ItemDefinition);
				}
				break;
			}
		}
	}
	
	return SelectedItems;
}

bool AF4SpawnVolume::GetRandomGroundPoint(FVector& OutLocation)
{
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent);
	
	FVector TraceStart = RandomPoint;
	TraceStart.Z = BoxOrigin.Z + BoxExtent.Z; // 박스 천장 높이에서부터
	FVector TraceEnd = RandomPoint;
	TraceEnd.Z = BoxOrigin.Z - BoxExtent.Z - 1000.0f; // 박스 바닥 아래까지
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActor(this);
	
	// WorldStatic 채널(바닥검사)
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_WorldStatic,
		QueryParams
	);
	
	if (bHit)
	{
		// 바닥에 묻힘 방지
		OutLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, 10.0f);
		return true;
	}
	
	return false;
}

void AF4SpawnVolume::OnItemsLoaded(TArray<TSoftObjectPtr<UF4ItemDefinition>> RolledItems)
{
	for (const TSoftObjectPtr<UF4ItemDefinition>& SoftPtr : RolledItems)
	{
		if (UF4ItemDefinition* ItemDef = SoftPtr.Get())
		{
			TrySpawnItem(ItemDef);
		}
	}
}

void AF4SpawnVolume::TrySpawnItem(UF4ItemDefinition* ItemDefinition)
{
	const UF4ItemFragment_Spawnable* SpawnableFrag = ItemDefinition->FindFragmentByClass<UF4ItemFragment_Spawnable>();
	if (!SpawnableFrag)
	{
		return;
	}

	FVector SpawnLocation;
	if (!GetRandomGroundPoint(SpawnLocation))
	{
		return;
	}

	FRotator SpawnRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
	AF4PickupActor* NewItem = GetWorld()->SpawnActorDeferred<AF4PickupActor>(
		SpawnableFrag->PickupActorClass,
		FTransform(SpawnRotation, SpawnLocation)
	);

	if (NewItem)
	{
		const int32 Quantity = FMath::RandRange(SpawnableFrag->MinSpawnQuantity, SpawnableFrag->MaxSpawnQuantity) * SpawnableFrag->QuantityMultiplier;

		NewItem->ItemQuantity = Quantity;
		NewItem->InitializePickup(ItemDefinition);
		NewItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
	}
}
