#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


class UButton;

UCLASS()
class F4_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	
	
	UFUNCTION()
	void OnStartClicked();
	
	UFUNCTION()
	void OnOptionClicked(); 
	
	UFUNCTION()
	void OnQuitClicked();
	
private:
	UPROPERTY(meta=(BindWidget))
	UButton* StartButton; 
	
	UPROPERTY(meta=(BindWidget))
	UButton* OptionButton; 
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton; 
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LobbyMapName = NAME_None; 
};
