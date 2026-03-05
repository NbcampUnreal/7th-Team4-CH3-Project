#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathMenuWidget.generated.h"

class UButton;

UCLASS()
class F4_API UDeathMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnContinueClicked();
	
	UFUNCTION()
	void OnQuitClicked(); 
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* ContinueButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton; 
	
	UPROPERTY(EditAnywhere)
	FName LobbyLevelName; 
	
	UPROPERTY(EditAnywhere)
	FName MainMenuLevelName; 
};
