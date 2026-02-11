#include "Items/Weapons/F4WeaponActor.h"

#include "Items/Weapons/F4WeaponDataAsset.h"

AF4WeaponActor::AF4WeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	MainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMeshComponent->SetupAttachment(RootScene);
	MainMeshComponent->SetCollisionProfileName(TEXT("NoCollision")); // 장착용이므로 충돌 끔

	AttachmentMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachmentMesh"));
	AttachmentMeshComponent->SetupAttachment(MainMeshComponent);
	AttachmentMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void AF4WeaponActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// 에디터에서 드래그 앤 드롭으로 데이터를 넣었을 때 미리보기 갱신
	if (WeaponData)
	{
		InitializeWeapon(WeaponData);
	}
}

void AF4WeaponActor::InitializeWeapon(const UF4WeaponDataAsset* InWeaponData)
{
	if (!InWeaponData)
	{
		UE_LOGFMT(LogTemp, Error, "WeaponBase: Invalid Data Asset provided!");
		return;
	}
	
	WeaponData = InWeaponData;
	
	if (WeaponData->MainMesh)
	{
		MainMeshComponent->SetStaticMesh(WeaponData->MainMesh);
	}
	
	if (WeaponData->WeaponType == EWeaponType::Gun)
	{
		if (WeaponData->AttachmentMesh)
		{
			AttachmentMeshComponent->SetStaticMesh(WeaponData->AttachmentMesh);
			AttachmentMeshComponent->SetVisibility(true);
		}
	}
	else
	{
		AttachmentMeshComponent->SetStaticMesh(nullptr);
		AttachmentMeshComponent->SetVisibility(false);
	}
}
