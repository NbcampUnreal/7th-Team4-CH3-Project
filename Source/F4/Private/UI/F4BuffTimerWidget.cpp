#include "UI/F4BuffTimerWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/F4BuffBarWidget.h"

void UF4BuffTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UF4BuffTimerWidget::StartBuffTimer(FActiveGameplayEffectHandle InHandle, UTexture2D* InIcon)
{
	if (!InHandle.IsValid())
	{
		return;
	}
	
	EffectHandle = InHandle;
	ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerPawn());
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
	
	if (bIsActive && ASC.IsValid())
	{
		const FActiveGameplayEffect* ActiveGE = ASC->GetActiveGameplayEffect(EffectHandle);
		if (!ActiveGE)
		{
			bIsActive = false;
			RemoveFromParent();
			return;
		}
		
		float RemainingTime = ActiveGE->GetTimeRemaining(GetWorld()->GetTimeSeconds());
		
		if (TimeText)
		{
			FString TimeString = FString::Printf(TEXT("%.1f초"), FMath::Max(0.0f, RemainingTime));
			TimeText->SetText(FText::FromString(TimeString));
		}
		
		if (RemainingTime <= 0.0f)
		{
			bIsActive = false;
			RemoveFromParent();
		}
	}
}
