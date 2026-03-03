#include "Enviroment/LevelMoveActor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "System/F4GameInstance.h"


ALevelMoveActor::ALevelMoveActor()
{
 	PrimaryActorTick.bCanEverTick = false;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionSphere);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);
}

void ALevelMoveActor::DoInteract(AActor* Interactor)
{
	APawn* InteractorPawn = Cast<APawn>(Interactor);
	
	if (InteractorPawn && InteractorPawn->IsPlayerControlled())
	{
		UF4GameInstance* GI = Cast<UF4GameInstance>(GetGameInstance());
		if (GI)
		{
			GI->MoveToLevel(TargetLevelName);
		}	
	}
}

void ALevelMoveActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WidgetComponent->SetVisibility(true);
}

void ALevelMoveActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	WidgetComponent->SetVisibility(false);
}

