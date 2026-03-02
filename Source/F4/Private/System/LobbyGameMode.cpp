#include "System/LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "System/F4GameState.h"

ALobbyGameMode::ALobbyGameMode()
{
	GameStateClass = AF4GameState::StaticClass();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (AF4GameState* GS = GetGameState<AF4GameState>())
	{
		GS->SetTimeFlows(false);
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ALobbyGameMode::StartMission(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), BattleLevelName);
}
