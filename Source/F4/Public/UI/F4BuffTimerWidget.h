#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4BuffTimerWidget.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;

UCLASS()
class F4_API UF4BuffTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI|Buff")
	void StartBuffTimer(float InDuration, UTexture2D* InIcon);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BuffIcon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	
private:
	float CurrentTime;
	bool bIsActive = false;
};
