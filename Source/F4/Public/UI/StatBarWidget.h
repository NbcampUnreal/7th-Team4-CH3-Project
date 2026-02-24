#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class F4_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateStatBar(float Current, float Max);
	
	void ToggleStatBar(bool bDraw);
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StatBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bar")
	UTextBlock* StatText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bar")
	float Ratio;
};
