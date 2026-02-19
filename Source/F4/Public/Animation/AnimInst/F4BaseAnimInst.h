// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "F4BaseAnimInst.generated.h"

class UCharacterMovementComponent;

UCLASS()
class F4_API UF4BaseAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	 virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	bool ShouldMove(); 	
	
	float CalculateDirection();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|State")
	bool bIsAiming = false;
	
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

	FGameplayTag AimingTag;
};
