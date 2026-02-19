#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "F4PickupActor.generated.h"

class UF4WeaponDataAsset; // TODO: 나중에는 UItemDataAsset으로 변경
class USphereComponent;

UCLASS()
class F4_API AF4PickupActor : public AActor, public IInteractable
{
	GENERATED_BODY()
    
public:    
	AF4PickupActor();

	virtual void DoInteract(AActor* Interactor) override;
	
	virtual FText GetInteractionText() const override;
	
	void InitializePickup(const UF4WeaponDataAsset* InItemData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> SubMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemData")
	TObjectPtr<const UF4WeaponDataAsset> ItemData;
};