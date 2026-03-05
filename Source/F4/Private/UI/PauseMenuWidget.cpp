#include "UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &ThisClass::OnContinueClicked);
	}
	
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &ThisClass::OnSettingClicked);
	}
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMainMenuClicked);
	}
	
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClicked);
	}
}

void UPauseMenuWidget::OnContinueClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	RemoveFromParent();
}

void UPauseMenuWidget::OnSettingClicked()
{
}

void UPauseMenuWidget::OnMainMenuClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}

void UPauseMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
