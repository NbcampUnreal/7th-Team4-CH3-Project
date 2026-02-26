// Fill out your copyright notice in the Description page of Project Settings.

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
	// 플레이어 최초 입장 
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// 전투맵 입장 
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void StartMission(APlayerController* PlayerController);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LobbyGameMode | Level")
	FName BattleLevelName;
	
};
