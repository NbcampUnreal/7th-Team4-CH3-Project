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

	SurvivalTimer += DeltaTime;

	if (!bTimeFlows)
	{
		return;
	}

	TimeOfDay += DeltaTime * TimeSpeed / 60.f;
	if (TimeOfDay >= 24.0f)
	{
		TimeOfDay -= 24.0f;
	}

	int32 CurrentHour = FMath::FloorToInt(TimeOfDay);
	int32 CurrentMinute = FMath::FloorToInt((TimeOfDay - CurrentHour) * 60.0f);

	if (CurrentHour != LastHour || CurrentMinute != LastMinute)
	{
		LastHour = CurrentHour;
		LastMinute = CurrentMinute;
		OnTimeChanged.Broadcast(CurrentHour, CurrentMinute);
	}
}

bool AF4GameState::IsDayTime() const
{
	return (TimeOfDay >= DawnTime && TimeOfDay <= DuskTime);
}

void AF4GameState::SetTimeOfDay(float NewTimeOfDay)
{
	TimeOfDay = FMath::Clamp(NewTimeOfDay, 0.f, 24.f);

	LastHour = FMath::FloorToInt(TimeOfDay);
	LastMinute = FMath::FloorToInt((TimeOfDay - LastHour) * 60.0f);
	OnTimeChanged.Broadcast(LastHour, LastMinute);
}

void AF4GameState::SetTimeFlows(bool bFlows)
{
	bTimeFlows = bFlows;
}
