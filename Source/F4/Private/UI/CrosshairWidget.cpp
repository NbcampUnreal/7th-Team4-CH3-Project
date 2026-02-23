// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CrosshairWidget.h"

#include "Components/Image.h"

void UCrosshairWidget::UpdateCrosshair(const float Value)
{
	if (!LineTop || !LineBottom ||!LineLeft || !LineRight) return;
	
	float Offset = FMath::Clamp(Value, 0.f, 100.f); 
	
	LineTop->SetRenderTranslation(FVector2D(0, -Offset));
	LineBottom->SetRenderTranslation(FVector2D(0, Offset));
	LineLeft->SetRenderTranslation(FVector2D(-Offset, 0));
	LineRight->SetRenderTranslation(FVector2D(Offset, 0));
}

void UCrosshairWidget::ToggleCrosshair(bool bDraw)
{
	if (bDraw)
	{
		LineTop->SetVisibility(ESlateVisibility::Visible);
		LineBottom->SetVisibility(ESlateVisibility::Visible);
		LineLeft->SetVisibility(ESlateVisibility::Visible);
		LineRight->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LineTop->SetVisibility(ESlateVisibility::Hidden);
		LineBottom->SetVisibility(ESlateVisibility::Hidden);
		LineLeft->SetVisibility(ESlateVisibility::Hidden);
		LineRight->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UCrosshairWidget::ToggleDot(bool bDraw)
{
	Dot->SetVisibility(bDraw ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
