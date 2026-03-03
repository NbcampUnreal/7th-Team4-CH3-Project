#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
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
	void AddBuff(FActiveGameplayEffectHandle EffectHandle, UTexture2D* BuffIconTexture);
	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnBuffAppliedCallback(FActiveGameplayEffectHandle EffectHandle, UTexture2D* BuffIcon);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> BuffBox;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI|Buff")
	TSubclassOf<UF4BuffTimerWidget> BuffTimerWidgetClass;
};
