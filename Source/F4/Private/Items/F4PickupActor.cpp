#include "Items/F4PickupActor.h"
#include "Components/SphereComponent.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Items/Weapons/F4WeaponDataAsset.h"

AF4PickupActor::AF4PickupActor()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	SetRootComponent(CollisionSphere);
	
	CollisionSphere->SetCollisionProfileName(TEXT("Custom"));
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ItemMeshComp->SetupAttachment(RootComponent);
	ItemMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	
	SubMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMeshComp"));
	SubMeshComp->SetupAttachment(RootComponent);
	SubMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void AF4PickupActor::DoInteract(AActor* Interactor)
{
	AF4PlayerCharacter* Character = Cast<AF4PlayerCharacter>(Interactor);
	if (Character && ItemData)
	{
		// TODO: GA_ProcessItemPickup으로 모듈화
		Character->ProcessItemPickup(ItemData);
		
		Destroy();
	}
}

FText AF4PickupActor::GetInteractionText() const
{
	return FText::FromString(TEXT("Player"));
}

void AF4PickupActor::InitializePickup(const UF4ItemDataAsset* InItemData)
{
	if (!InItemData) return;
	ItemData = InItemData;
	
	if (ItemData->PickupMesh)
	{
		ItemMeshComp->SetStaticMesh(ItemData->PickupMesh);
	}
	
	if (const UF4WeaponDataAsset* WeaponData = Cast<UF4WeaponDataAsset>(ItemData))
	{
		if (WeaponData->WeaponType == EWeaponType::Gun && WeaponData->MagazineMesh)
		{
			SubMeshComp->SetStaticMesh(WeaponData->MagazineMesh);
			SubMeshComp->SetVisibility(true);
			
			if (!WeaponData->MagazineSocketName.IsNone())
			{
				SubMeshComp->AttachToComponent(
					ItemMeshComp,
					FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponData->MagazineSocketName
				);
			}
		}
	}
	else
	{
		SubMeshComp->SetStaticMesh(nullptr);
		// SubMeshComp->SetVisibility(false);
	}
}
