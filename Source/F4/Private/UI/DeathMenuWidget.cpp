#include "UI/DeathMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "System/BattleGameMode.h"

void UDeathMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &ThisClass::OnContinueClicked);
	}
	
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClicked);
	}
}

void UDeathMenuWidget::OnContinueClicked()
{
	ABattleGameMode* GM = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
	GM->HandlePlayerDeath(GetOwningPlayer());
}

void UDeathMenuWidget::OnQuitClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}
