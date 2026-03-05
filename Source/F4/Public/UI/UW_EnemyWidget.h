#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_EnemyWidget.generated.h"

class UProgressBar;

UCLASS()
class F4_API UUW_EnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StatBar;
	
	void UpdateStatBar(float Current, float Max);
	void UpdateWidgetVisibility(bool bIsVisible);
};
