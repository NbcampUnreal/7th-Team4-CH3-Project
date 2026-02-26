// Fill out your copyright notice in the Description page of Project Settings.


#include "System/BattleGameMode.h"

#include "Characters/Player/F4PlayerCharacter.h"
#include "Characters/Player/F4PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "System/F4GameInstance.h"

ABattleGameMode::ABattleGameMode()
{
}

void ABattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	UF4GameInstance* GameInstance = Cast<UF4GameInstance>(GetGameInstance());
	AF4PlayerCharacter* PlayerCharacter = Cast<AF4PlayerCharacter>(NewPlayer->GetPawn());
	
	if (GameInstance && PlayerCharacter)
	{
		// 플레이어 인벤토리 복원
		
	}
}

void ABattleGameMode::OnDifficultyPhaseChanged(int32 NewPhase)
{
}

void ABattleGameMode::HandlePlayerDeath(APlayerController* PlayerController)
{
	UF4GameInstance* GameInstance = Cast<UF4GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		// 데이터 초기화 
		GameInstance->WipeData(); 
	}
	
	if (AF4PlayerController* F4Controller =Cast<AF4PlayerController>(PlayerController))
	{
		// 컨트롤러 패배 처리
	}
	
	// 로비로 이동 
	UGameplayStatics::OpenLevel(GetWorld(), LobbyLevelName);
	
}

void ABattleGameMode::HandlePlayerEvacuation(APlayerController* PlayerController)
{
	//플레이어 귀환 
	UF4GameInstance* GameInstance = Cast<UF4GameInstance>(GetGameInstance());
	AF4PlayerCharacter* PlayerCharacter = Cast<AF4PlayerCharacter>(PlayerController->GetPawn());
	
	// 캐릭터 인벤토리 저장 
	
	
	// 로비로 이동 
	if (LobbyLevelName.IsValid())
	{
		UGameplayStatics::OpenLevel(GetWorld(), LobbyLevelName);
	}
}
