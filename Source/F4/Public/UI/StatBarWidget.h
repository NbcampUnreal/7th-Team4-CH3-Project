// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

class UProgressBar;

UCLASS()
class F4_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateStatBar(float InRatio);
	
	void ToggleStatBar(bool bDraw);
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StatBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bar")
	float Ratio;
};
