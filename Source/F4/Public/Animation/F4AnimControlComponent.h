#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F4AnimControlComponent.generated.h"

class UF4ItemInstance;
class UAnimInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class F4_API UF4AnimControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UF4AnimControlComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponChanged(UF4ItemInstance* NewItem);

	void UpdateAnimationLayer(UF4ItemInstance* NewItem);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> DefaultUnarmedLayer;

private:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CachedMesh;
};
