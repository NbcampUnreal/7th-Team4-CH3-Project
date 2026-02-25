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

FTransform AF4WeaponActor::GetMuzzleTransform() const
{
	if (MainMeshComponent && MainMeshComponent->DoesSocketExist(TEXT("Muzzle")))
	{
		return MainMeshComponent->GetSocketTransform(TEXT("Muzzle"));
	}
	
	return GetActorTransform();
}
