// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GaugeWidget.h"
#include "Components/Image.h"


void UGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GaugeImage && GaugeMaterial)
	{
		InstanceDynamic = UMaterialInstanceDynamic::Create(GaugeMaterial, this);
		GaugeImage->SetBrushFromMaterial(InstanceDynamic);
		ToggleGauge(false); 
	}
}

void UGaugeWidget::UpdateGauge(float InRatio)
{
	if (InstanceDynamic)
	{
		Ratio = FMath::Clamp(InRatio, 0.f, 1.f);
		InstanceDynamic->SetScalarParameterValue("Ratio", Ratio);
		
		ToggleGauge(Ratio < 1.f);
	}
	
}

void UGaugeWidget::ToggleGauge(bool bDraw)
{
	if (!InstanceDynamic) return;
	
	GaugeImage->SetVisibility(bDraw ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
