// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UStatBarWidget::UpdateStatBar(float Current, float Max)
{
	Ratio = FMath::Clamp(Current / Max, 0.0f, 1.0f);
	
	if (StatBar) StatBar->SetPercent(Ratio);
	
	if (StatText)
	{
		FString StringValue = FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(Current), FMath::RoundToInt(Max));
		StatText->SetText(FText::FromString(StringValue));
	}
	
}

void UStatBarWidget::ToggleStatBar(bool bDraw)
{
	if (!StatBar) return;
	
	StatBar->SetVisibility(bDraw ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
