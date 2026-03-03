#include "UI/F4BuffBarWidget.h"
#include "UI/F4BuffTimerWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Inventory/F4BuffComponent.h"

void UF4BuffBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (APawn* OwningPawn = GetOwningPlayerPawn())
		{
			if (UF4BuffComponent* BuffComp = OwningPawn->FindComponentByClass<UF4BuffComponent>())
			{
				BuffComp->OnBuffApplied.AddDynamic(this, &UF4BuffBarWidget::OnBuffAppliedCallback);
			}
		}
	}
}

void UF4BuffBarWidget::OnBuffAppliedCallback(FActiveGameplayEffectHandle EffectHandle, UTexture2D* BuffIcon)
{
	AddBuff(EffectHandle, BuffIcon);
}

void UF4BuffBarWidget::AddBuff(FActiveGameplayEffectHandle EffectHandle, UTexture2D* BuffIconTexture)
{
	if (BuffTimerWidgetClass && BuffBox)
	{
		UF4BuffTimerWidget* NewBuff = CreateWidget<UF4BuffTimerWidget>(GetOwningPlayer(), BuffTimerWidgetClass);
		
		if (NewBuff)
		{
			NewBuff->StartBuffTimer(EffectHandle, BuffIconTexture);
			UHorizontalBoxSlot* BoxSlot = BuffBox->AddChildToHorizontalBox(NewBuff);
			
			if (BoxSlot)
			{
				BoxSlot->SetPadding(FMargin(0.0f, 0.0f, 10.0f, 0.0f));
			}
		}
	}
}
