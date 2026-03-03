#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Blueprint/UserWidget.h"
#include "F4BuffTimerWidget.generated.h"

class UAbilitySystemComponent;
class UTextBlock;
class UImage;
class UTexture2D;

UCLASS()
class F4_API UF4BuffTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI|Buff")
	void StartBuffTimer(FActiveGameplayEffectHandle InHandle, UTexture2D* InIcon);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BuffIcon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	
private:
	FActiveGameplayEffectHandle EffectHandle;
	
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	
	bool bIsActive = false;
};
