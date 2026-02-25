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

void AF4PickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (ItemDefinition)
	{
		const UF4ItemDefinition* DefCDO = GetDefault<UF4ItemDefinition>(ItemDefinition);

		const UF4ItemFragment_PickupVisual* VisualFrag = DefCDO->FindFragmentByClass<UF4ItemFragment_PickupVisual>();

		if (VisualFrag)
		{
			if (VisualFrag->PickupMesh)
			{
				ItemMeshComp->SetStaticMesh(VisualFrag->PickupMesh);
				ItemMeshComp->SetRelativeScale3D(VisualFrag->PickupScale);
			}

			if (VisualFrag->SubPickupMesh)
			{
				SubMeshComp->SetStaticMesh(VisualFrag->SubPickupMesh);
				SubMeshComp->AttachToComponent(
					ItemMeshComp,
					FAttachmentTransformRules::SnapToTargetIncludingScale,
					VisualFrag->SubMeshSocketName
				);
			}
			else
			{
				SubMeshComp->SetStaticMesh(nullptr);
			}
		}
	}
}

void AF4PickupActor::DoInteract(AActor* Interactor)
{
	if (!Interactor || !ItemDefinition)
	{
		return;
	}
	
	if (AF4PlayerCharacter* PlayerCharacter = Cast<AF4PlayerCharacter>(Interactor))
	{
		UF4InventoryComponent* InventoryComp = PlayerCharacter->FindComponentByClass<UF4InventoryComponent>();
		
		if (InventoryComp)
		{
			UF4ItemInstance* PickedItemInstance = NewObject<UF4ItemInstance>(InventoryComp);
			
			InventoryComp->AddItem(PickedItemInstance);
			
			Destroy();
		}
	}
}

FText AF4PickupActor::GetInteractionText() const
{
	return FText::FromString(TEXT("Player"));
}

void AF4PickupActor::InitializePickup(TSubclassOf<class UF4ItemDefinition> InItemDefinition)
{
	if (!InItemDefinition) return;
	ItemDefinition = InItemDefinition;

	const UF4ItemDefinition* DefCDO = GetDefault<UF4ItemDefinition>(ItemDefinition);
	const UF4ItemFragment_PickupVisual* VisualFrag = DefCDO->FindFragmentByClass<UF4ItemFragment_PickupVisual>();
	
	if (VisualFrag)
	{
		ItemMeshComp->SetStaticMesh(VisualFrag->PickupMesh);
		ItemMeshComp->SetRelativeScale3D(VisualFrag->PickupScale);
		
		if (VisualFrag->SubPickupMesh)
		{
			SubMeshComp->SetStaticMesh(VisualFrag->SubPickupMesh);
		}
		else
		{
			SubMeshComp->SetStaticMesh(nullptr);
		}
	}
}
