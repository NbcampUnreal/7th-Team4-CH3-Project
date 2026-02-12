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
		// 탄창이라면 처리
		if (WeaponData->MagazineMesh)
		{
			// 탄창 메시 설정
			AttachmentMeshComponent->SetStaticMesh(WeaponData->MagazineMesh);
			AttachmentMeshComponent->SetVisibility(true);
		}
		
		// 소켓에 스냅 시키기
		if (!WeaponData->MagazineSocketName.IsNone())
		{
			AttachmentMeshComponent->AttachToComponent(
				MainMeshComponent,
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				WeaponData->MagazineSocketName
			);
		}
		else
		{
			// 소캣 이름이 없을 경우 경고 로그띄우고 임시로 그냥 붙임
			UE_LOGFMT(LogTemp, Warning, "Weapon: Magazine Socket Name is missing in DataAsset!");
			
			AttachmentMeshComponent->AttachToComponent(
					MainMeshComponent, 
					FAttachmentTransformRules::SnapToTargetIncludingScale
					);
		}
	}
	else
	{
		// 근접 무기
		AttachmentMeshComponent->SetStaticMesh(nullptr);
		AttachmentMeshComponent->SetVisibility(false);
	}
}
