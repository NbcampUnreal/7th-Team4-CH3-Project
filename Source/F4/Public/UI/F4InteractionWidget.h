#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4InteractionWidget.generated.h"

class UTextBlock;

UCLASS()
class F4_API UF4InteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractText(const FText& InText);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractPromptText;
};
