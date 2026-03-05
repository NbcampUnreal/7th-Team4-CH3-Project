#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "LevelMoveActor.generated.h"

class UWidgetComponent;
class USphereComponent; 

UCLASS()
class F4_API ALevelMoveActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ALevelMoveActor();

	virtual void DoInteract(AActor* Interactor) override; 
	
	virtual FText GetInteractionText() const override {return FText::FromName(TargetLevelName);}
	
	virtual void BeginPlay() override; 

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelMoveActor")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelMoveActor")
	FName TargetLevelName;
};
