#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "F4GameState.generated.h"


UCLASS()
class F4_API AF4GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AF4GameState();
	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	int32 LocalDifficultyPhase = 0; 
	
protected:
	UPROPERTY()
	float SurvivalTimer = 0.f; 
	float DifficultyAccumulator = 0.0f;
	
	// 난이도 증가 주기
	const float DifficultyUpdateInterval = 20.0f;
};
