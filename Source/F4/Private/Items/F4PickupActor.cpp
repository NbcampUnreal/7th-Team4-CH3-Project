#include "Items/F4PickupActor.h"
#include "Components/SphereComponent.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemInstance.h"
#include "Items/F4ItemFragment_PickupVisual.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/F4InteractionWidget.h"

AF4PickupActor::AF4PickupActor()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);

	CollisionSphere->SetCollisionProfileName(TEXT("Custom"));
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionSphere->SetGenerateOverlapEvents(true);

	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ItemMeshComp->SetupAttachment(RootComponent);
	ItemMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	SubMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMeshComp"));
	SubMeshComp->SetupAttachment(RootComponent);
	SubMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	WidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f));
	WidgetComponent->SetVisibility(false);
}

void AF4PickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (ItemDefinition)
	{
		const UF4ItemFragment_PickupVisual* VisualFrag = ItemDefinition->FindFragmentByClass<UF4ItemFragment_PickupVisual>();

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

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AF4PickupActor::OnSphereBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AF4PickupActor::OnSphereEndOverlap);
}

void AF4PickupActor::DoInteract(AActor* Interactor)
{
	if (!Interactor || !ItemDefinition)
	{
		UE_LOGFMT(LogTemp, Warning, "Interact failed: Interactor 또는 ItemDefinition이 null");
		return;
	}
	
	if (AF4PlayerCharacter* PlayerCharacter = Cast<AF4PlayerCharacter>(Interactor))
	{
		UF4InventoryComponent* InventoryComp = PlayerCharacter->FindComponentByClass<UF4InventoryComponent>();
		
		if (InventoryComp)
		{
			UF4ItemInstance* PickedItemInstance = NewObject<UF4ItemInstance>(InventoryComp);
			
			PickedItemInstance->ItemDefinition = ItemDefinition;
			
			UE_LOGFMT(LogTemp, Warning, "아이템 추가 시도: {0}", ItemDefinition->GetName());
			
			int32 BeforeCount = InventoryComp->GetInventoryItems().Num();
			
			InventoryComp->AddItem(PickedItemInstance);
			
			int32 AfterCount = InventoryComp->GetInventoryItems().Num();
			
			if (AfterCount > BeforeCount)
			{
				UE_LOGFMT(LogTemp, Warning, "아이템 추가 성공 (이전: {0}, 현재: {1})", BeforeCount, AfterCount);
			}
			else
			{
				UE_LOGFMT(LogTemp, Warning, "스택형이므로 합쳐짐");
			}
			
			Destroy();
		}
		else
		{
			UE_LOGFMT(LogTemp, Error, "Interact 실패: {0}에게 InventoryComponent가 없음.", PlayerCharacter->GetName());
		}
	}
	else
	{
		UE_LOGFMT(LogTemp, Warning, "Interact 실패: 상호작용 대상이 PlayerCharacter가 아님.");
	}
}

FText AF4PickupActor::GetInteractionText() const
{
	return FText::FromString(TEXT("Player"));
}

void AF4PickupActor::InitializePickup(UF4ItemDefinition* InItemDefinition)
{
	if (!InItemDefinition) return;
	ItemDefinition = InItemDefinition;
	
	const UF4ItemFragment_PickupVisual* VisualFrag = ItemDefinition->FindFragmentByClass<UF4ItemFragment_PickupVisual>();
	
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

void AF4PickupActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA<AF4PlayerCharacter>())
	{
		if (UF4InteractionWidget* InteractionWidget = Cast<UF4InteractionWidget>(WidgetComponent->GetUserWidgetObject()))
		{
			const UF4ItemFragment_UI* UIFragment = ItemDefinition->FindFragmentByClass<UF4ItemFragment_UI>();
			if (UIFragment)
			{
				FText FormattedText = FText::Format(FText::FromString(TEXT("F - {0} 줍기")), UIFragment->ItemName);
				InteractionWidget->SetInteractText(FormattedText);
			}
		}

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->PlayerCameraManager)
		{
			FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
			FVector WidgetLocation = WidgetComponent->GetComponentLocation();

			FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);

			// 깔끔하게 좌우만 맞추고 싶다면 아래처럼 (위젯이 뒤집히는 것 방지)
			// LookAtRot.Pitch = 0.0f;
			// LookAtRot.Roll = 0.0f;

			WidgetComponent->SetWorldRotation(LookAtRot);
		}

		WidgetComponent->SetVisibility(true);
	}
}

void AF4PickupActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA<AF4PlayerCharacter>())
	{
		WidgetComponent->SetVisibility(false);
	}
}
