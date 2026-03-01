#include "Items/F4SpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Items/F4PickupActor.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Spawnable.h"
#include "Items/F4ItemFragment_PickupVisual.h"
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
	UAssetManager& AssetManager = UAssetManager::Get();

	TArray<FPrimaryAssetId> AllItemIds;
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("ItemDefinition"), AllItemIds);

	if (AllItemIds.IsEmpty())
	{
		UE_LOGFMT(LogTemp, Warning, "SpawnVolume: 스폰할 아이템 ID 목록을 찾을 수 없습니다.");
		return;
	}

	TArray<FPrimaryAssetId> SelectedAssetIds;
	for (int32 i = 0; i < SpawnCount; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, AllItemIds.Num() - 1);
		SelectedAssetIds.Add(AllItemIds[RandomIndex]);
	}

	FStreamableDelegate LoadDelegate = FStreamableDelegate::CreateUObject(this, &AF4SpawnVolume::OnItemsLoaded, SelectedAssetIds);
	AssetManager.LoadPrimaryAssets(SelectedAssetIds, TArray<FName>(), LoadDelegate);
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

void AF4SpawnVolume::OnItemsLoaded(TArray<FPrimaryAssetId> LoadedAssetIds)
{
	UAssetManager& AssetManager = UAssetManager::Get();

    for (const FPrimaryAssetId& AssetId : LoadedAssetIds)
    {
	    if (UF4ItemDefinition* LoadedItemDefinition = Cast<UF4ItemDefinition>(AssetManager.GetPrimaryAssetObject(AssetId)))
        {
	        TrySpawnItem(LoadedItemDefinition);
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

	const UF4ItemFragment_PickupVisual* VisualFrag = ItemDefinition->FindFragmentByClass<UF4ItemFragment_PickupVisual>();
	if (!VisualFrag)
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
