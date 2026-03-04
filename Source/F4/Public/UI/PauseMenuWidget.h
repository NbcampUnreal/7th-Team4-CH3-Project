#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class F4_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnContinueClicked();
	
	UFUNCTION()
	void OnSettingClicked();
	
	UFUNCTION()
	void OnMainMenuClicked();
	
	UFUNCTION()
	void OnQuitClicked();
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SettingsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
	
	UPROPERTY(EditAnywhere)
	FName MainMenuLevelName;
};
