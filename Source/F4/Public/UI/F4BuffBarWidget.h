#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4BuffBarWidget.generated.h"

class UF4BuffTimerWidget;
class UHorizontalBox;

UCLASS()
class F4_API UF4BuffBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI|Buff")
	void AddBuff(float Duration, UTexture2D* BuffIconTexture);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> BuffBox;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI|Buff")
	TSubclassOf<UF4BuffTimerWidget> BuffTimerWidgetClass;
};
