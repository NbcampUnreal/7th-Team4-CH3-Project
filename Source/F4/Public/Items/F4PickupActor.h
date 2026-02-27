#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "F4PickupActor.generated.h"

class UWidgetComponent;
class UF4ItemDefinition;
class UF4ItemDataAsset;
class USphereComponent;

UCLASS()
class F4_API AF4PickupActor : public AActor, public IInteractable
{
	GENERATED_BODY()
    
public:    
	AF4PickupActor();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void DoInteract(AActor* Interactor) override;
	
	virtual FText GetInteractionText() const override;
	
	// 적이 드랍 아이템을 드랍하는 경우 사용
	void InitializePickup(UF4ItemDefinition* InItemDefinition);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> SubMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UF4ItemDefinition> ItemDefinition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
