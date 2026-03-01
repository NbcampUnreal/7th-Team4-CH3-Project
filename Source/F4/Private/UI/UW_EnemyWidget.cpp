#include "UI/UW_EnemyWidget.h"
#include "Components/ProgressBar.h"

void UUW_EnemyWidget::UpdateStatBar(float Current, float Max)
{
	if (StatBar && Max > 0.f)
	{
		StatBar->SetPercent(Current / Max);
	}
}

void UUW_EnemyWidget::UpdateWidgetVisibility(bool bIsVisible)
{
	ESlateVisibility NewVisibility = bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
	SetVisibility(NewVisibility);
}
