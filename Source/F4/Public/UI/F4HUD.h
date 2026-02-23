// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4HUD.generated.h"

class UCrosshairWidget; 
class UStatBarWidget;
class UAbilitySystemComponent;;

UCLASS()
class F4_API UF4HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void UpdateCrosshair(float InDeltaTime);
	void AddRecoilImpulse(float ImpulseAmount);
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCrosshairWidget> Crosshair;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatBarWidget> HealthBar;
	
	UPROPERTY()
	TObjectPtr<APawn> Owner;

	UPROPERTY()
	UAbilitySystemComponent* OwnerASC;
	
	bool bAiming = false; 
	
	float CurrentSpread = 15.f;
	float RecoilSpread = 0.f;
};
