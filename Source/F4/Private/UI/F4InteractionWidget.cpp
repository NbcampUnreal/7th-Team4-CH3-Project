#include "UI/F4InteractionWidget.h"
#include "Components/TextBlock.h"

void UF4InteractionWidget::SetInteractText(const FText& InText)
{
	if (InteractPromptText)
	{
		InteractPromptText->SetText(InText);
	}
}
