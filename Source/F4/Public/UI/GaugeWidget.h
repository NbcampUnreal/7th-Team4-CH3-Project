// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GaugeWidget.generated.h"

class UImage;

UCLASS()
class F4_API UGaugeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void UpdateGauge(float InRatio);
	
	void ToggleGauge(bool bDraw);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* GaugeImage;
	
	UPROPERTY(EditDefaultsOnly, Category= "Gauge")
	UMaterialInterface* GaugeMaterial;
	
	UPROPERTY()
	UMaterialInstanceDynamic* InstanceDynamic;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Gauge")
	float Ratio = 1.f;
};
