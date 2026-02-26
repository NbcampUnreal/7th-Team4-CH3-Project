#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "F4GameState.generated.h"


UCLASS()
class F4_API AF4GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
	
	
public:
	UPROPERTY()
	float SurvivalTimer = 0.f; 
	
	UPROPERTY()
	int32 DifficultyPhase = 1; 
};
