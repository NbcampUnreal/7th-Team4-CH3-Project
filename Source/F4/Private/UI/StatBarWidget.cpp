// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatBarWidget.h"

#include "Components/ProgressBar.h"

void UStatBarWidget::UpdateStatBar(float InRatio)
{
	StatBar->SetPercent(InRatio);
}

void UStatBarWidget::ToggleStatBar(bool bDraw)
{
	if (!StatBar) return;
	
	StatBar->SetVisibility(bDraw ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
