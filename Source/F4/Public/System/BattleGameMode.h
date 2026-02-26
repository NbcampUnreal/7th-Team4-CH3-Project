#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BattleGameMode.generated.h"

UCLASS()
class F4_API ABattleGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABattleGameMode();
	
	// 플레이어 최초 입장 
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// 시간에 따른 난이도 변경 
	void OnDifficultyPhaseChanged(int32 NewPhase);
	
	// 플레이어 사망 
	void HandlePlayerDeath(APlayerController* PlayerController);
	
	// 플레이어 귀환
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void HandlePlayerEvacuation(APlayerController* PlayerController); 
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BattleGameMode | Level")
	FName LobbyLevelName; 
};
