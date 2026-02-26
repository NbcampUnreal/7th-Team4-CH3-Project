#include "System/LobbyGameMode.h"

#include "Characters/Player/F4PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "System/F4GameInstance.h"

ALobbyGameMode::ALobbyGameMode()
{
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	UF4GameInstance* GameInstance = Cast<UF4GameInstance>(GetGameInstance());
	AF4PlayerCharacter* PlayerCharacter = Cast<AF4PlayerCharacter>(NewPlayer->GetPawn());
	
	if (GameInstance && PlayerCharacter)
	{
		// TODO : 인벤토리 데이터 저장 
		
	}
}

void ALobbyGameMode::StartMission(APlayerController* PlayerController)
{
	if (!PlayerController) return;
	
	AF4PlayerCharacter* Character = Cast<AF4PlayerCharacter>(PlayerController->GetPawn());
	UF4GameInstance* GameInstance = Cast<UF4GameInstance>(GetGameInstance());
	
	if (GameInstance && Character)
	{
		// 아이템 정보 저장 
		GameInstance->SaveData(); 
		
		// 맵 이동 
		UGameplayStatics::OpenLevel(GetWorld(), BattleLevelName);
	}
}
