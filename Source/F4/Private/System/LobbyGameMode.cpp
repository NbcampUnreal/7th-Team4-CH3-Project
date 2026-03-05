#include "System/LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "System/F4GameInstance.h"
#include "System/F4GameState.h"

ALobbyGameMode::ALobbyGameMode()
{
	GameStateClass = AF4GameState::StaticClass();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Current GameMode is Lobby GameMode."));

	if (AF4GameState* GS = GetGameState<AF4GameState>())
	{
		GS->SetTimeFlows(false);
	}
	
	UF4GameInstance* GI = Cast<UF4GameInstance>(GetGameInstance()); 
	if (GI)
	{
		if (LobbyBGM)
		{
			UE_LOG(LogTemp, Warning, TEXT("Lobby BGM Started"));
			GI->PlayBGM(LobbyBGM, bFade);
		}
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
