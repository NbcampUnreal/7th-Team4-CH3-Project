#include "UI/F4BuffBarWidget.h"
#include "UI/F4BuffTimerWidget.h"
#include "Components/HorizontalBox.h"

void UF4BuffBarWidget::AddBuff(float Duration, UTexture2D* BuffIconTexture)
{
	if (BuffTimerWidgetClass && BuffBox)
	{
		UF4BuffTimerWidget* NewBuff = CreateWidget<UF4BuffTimerWidget>(GetOwningPlayer(), BuffTimerWidgetClass);
		
		if (NewBuff)
		{
			NewBuff->StartBuffTimer(Duration, BuffIconTexture);
			
			BuffBox->AddChildToHorizontalBox(NewBuff);
		}
	}
}
