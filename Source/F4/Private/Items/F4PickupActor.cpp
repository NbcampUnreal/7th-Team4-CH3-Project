#include "Items/F4PickupActor.h"
#include "Components/SphereComponent.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Items/Weapons/F4WeaponDataAsset.h"
#include "System/F4GameplayTags.h"

AF4PickupActor::AF4PickupActor()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
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
	UAbilitySystemComponent* ASC = Interactor->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC && Interactor)
	{
		FGameplayEventData Payload;
		Payload.Instigator = this;
		Payload.Target = Interactor;
		Payload.OptionalObject = ItemData;
		
		ASC->HandleGameplayEvent(F4GameplayTags::Event_Interaction_Pickup, &Payload);
	}
	
	Destroy();
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
		ItemMeshComp->SetRelativeScale3D(ItemData->PickupScale);
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
