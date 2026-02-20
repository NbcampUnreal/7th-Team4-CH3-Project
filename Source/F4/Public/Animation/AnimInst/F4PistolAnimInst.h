// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInst/F4BaseAnimInst.h"
#include "F4PistolAnimInst.generated.h"


UCLASS()
class F4_API UF4PistolAnimInst : public UF4BaseAnimInst
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(const float DeltaSeconds) override;
	
	void UpdateAimOffset();
	
	void UpdateAiming(); 
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bAiming = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	float AimPitch = 0.f; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	float AimYaw = 0.f; 
};
