// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlot.generated.h"

class UImage; 
class UTextBlock;

UCLASS()
class F4_API UQuickSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateSlot(); 

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage; 
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ButtonText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText; 
	
	
	
};
