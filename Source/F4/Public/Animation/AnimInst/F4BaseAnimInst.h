#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "Inventory/F4EquipmentComponent.h"
#include "F4BaseAnimInst.generated.h"

class UCharacterMovementComponent;
class UF4ItemInstance;

UCLASS()
class F4_API UF4BaseAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	 virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	bool ShouldMove(); 	
	
	float CalculateDirection();

	void BindToEquipmentComponent(UF4EquipmentComponent* EquipComp);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ref") 
	ACharacter* OwnerCharacter; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ref") 
	UCharacterMovementComponent* MovementComp; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Variable")
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Variable")
	float GroundSpeed; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Variable")
	float Direction; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Variable")
	bool bIsFalling;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Variable")
	bool bShouldMove; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Montage | Variable")
	bool bIsPlayingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|State")
	bool bIsAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|State")
	bool bIsEquipped = false;

	FGameplayTag AimingTag;

	UFUNCTION()
	void HandleLayerChange(UF4ItemInstance* NewItem);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> DefaultUnarmedLayer;
};
