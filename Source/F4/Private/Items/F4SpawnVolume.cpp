#include "Items/F4SpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Items/F4PickupActor.h"
#include "Inventory/F4ItemDefinition.h"
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
	
	SpawnItems();
}

void AF4SpawnVolume::SpawnItems()
{
	if (SpawnableItems.IsEmpty())
	{
		UE_LOGFMT(LogTemp, Warning, "SpawnVolume: SpawnableItems is empty in {0}", GetName());
		return;
	}

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		FVector SpawnLocation;
		if (!GetRandomGroundPoint(SpawnLocation))
		{
			continue;
		}

		int32 RandomIndex = FMath::RandRange(0, SpawnableItems.Num() - 1);

		FF4SpawnItemData SelectedData = SpawnableItems[RandomIndex];

		if (!SelectedData.ItemDefinition) continue;

		TSubclassOf<AF4PickupActor> ClassToSpawn = SelectedData.OverridePickupClass ? SelectedData.OverridePickupClass : DefaultItemClassToSpawn;

		if (!ClassToSpawn) continue;

		FRotator SpawnRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

		AF4PickupActor* NewItem = GetWorld()->SpawnActorDeferred<AF4PickupActor>(
		   ClassToSpawn,
		   FTransform(SpawnRotation, SpawnLocation)
		);

		if (NewItem)
		{
			NewItem->ItemQuantity = SelectedData.Quantity;

			NewItem->InitializePickup(SelectedData.ItemDefinition);
			NewItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
		}
	}
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
