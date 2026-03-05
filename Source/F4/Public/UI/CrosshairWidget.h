#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UImage;


UCLASS()
class F4_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "UI|Crosshair")
	void UpdateCrosshair(const float Value); 
	
	UFUNCTION(BlueprintCallable, Category = "UI|Crosshair")
	void ToggleCrosshair(bool bDraw);
	
	UFUNCTION(BlueprintCallable, Category = "UI|Crosshair")
	void ToggleDot(bool bDraw);
	
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Dot; 
	
	UPROPERTY(meta = (BindWidget))
	UImage* LineTop;
	
	UPROPERTY(meta = (BindWidget))
	UImage* LineBottom;
	
	UPROPERTY(meta = (BindWidget))
	UImage* LineLeft;
	
	UPROPERTY(meta = (BindWidget))	
	UImage* LineRight;
};
