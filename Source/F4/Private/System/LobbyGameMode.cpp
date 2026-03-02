#include "System/LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
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
