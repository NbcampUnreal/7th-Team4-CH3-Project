#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"


UCLASS()
class F4_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void BeginPlay() override;

	// 플레이어 최초 입장
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// 전투맵 입장 
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void StartMission(APlayerController* PlayerController);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Lobby | Level")
	FName BattleLevelName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lobby | BGM")
	USoundBase* LobbyBGM;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lobby | BGM")
	float BGMVolume = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lobby | BGM")
	bool bFade = true; 
};
