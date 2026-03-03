#include "System/F4GameState.h"

AF4GameState::AF4GameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AF4GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SurvivalTimer += DeltaTime; 
	
	DifficultyAccumulator += DeltaTime;
	
	if (DifficultyAccumulator >= DifficultyUpdateInterval)
	{
		DifficultyAccumulator -= DifficultyUpdateInterval;
		
		LocalDifficultyPhase++;

		UE_LOG(LogTemp, Log, TEXT("Local Difficulty Increased! Current: %d (Survival Time: %.1f)"), 
			LocalDifficultyPhase, SurvivalTimer);
	}
}
