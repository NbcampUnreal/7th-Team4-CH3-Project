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
				
				/*if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1, 5.0f, FColor::Green, TEXT("버프 UI 바인딩 성공"));
				}*/
			}
		}
	}
}

void UF4BuffBarWidget::OnBuffAppliedCallback(float Duration, UTexture2D* BuffIcon)
{
	AddBuff(Duration, BuffIcon);
}

void UF4BuffBarWidget::AddBuff(float Duration, UTexture2D* BuffIconTexture)
{
	/*if (!BuffTimerWidgetClass)
	{
		FString ErrorMsg = FString::Printf(TEXT("[%s] 클래스가 비어있음"), *GetName());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 5.f, FColor::Red, ErrorMsg);
		}
		return;
	}*/
	
	if (BuffTimerWidgetClass && BuffBox)
	{
		UF4BuffTimerWidget* NewBuff = CreateWidget<UF4BuffTimerWidget>(GetOwningPlayer(), BuffTimerWidgetClass);
		
		if (NewBuff)
		{
			NewBuff->StartBuffTimer(Duration, BuffIconTexture);
			UHorizontalBoxSlot* BoxSlot = BuffBox->AddChildToHorizontalBox(NewBuff);
			
			if (BoxSlot)
			{
				BoxSlot->SetPadding(FMargin(0.0f, 0.0f, 10.0f, 0.0f));
			}

			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 5.f, FColor::Cyan, FString::Printf(TEXT("위젯 생성 완료")));
			}*/
		}
	}
}
