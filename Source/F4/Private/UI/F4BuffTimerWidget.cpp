#include "UI/F4BuffTimerWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

void UF4BuffTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}

void UF4BuffTimerWidget::StartBuffTimer(float InDuration, UTexture2D* InIcon)
{
	if (InDuration <= 0.0f)
	{
		return;
	}
	
	CurrentTime = InDuration;
	bIsActive = true;
	
	if (BuffIcon && InIcon)
	{
		BuffIcon->SetBrushFromTexture(InIcon);
	}
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UF4BuffTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsActive)
	{
		CurrentTime -= InDeltaTime;
		
		if (TimeText)
		{
			FString TimeString = FString::Printf(TEXT("%.1f"), FMath::Max(0.0f, CurrentTime));
			TimeText->SetText(FText::FromString(TimeString));
		}
	}
	
	if (CurrentTime <= 0.0f)
	{
		bIsActive = false;
		RemoveFromParent();
	}
}
