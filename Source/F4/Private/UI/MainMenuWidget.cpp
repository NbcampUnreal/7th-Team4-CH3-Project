// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::OnStartClicked);
	}

	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnOptionClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClicked);
	}
}

void UMainMenuWidget::OnStartClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName);
}

void UMainMenuWidget::OnOptionClicked()
{
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(
		this,
		nullptr,
		EQuitPreference::Quit,
		true
	);
}
