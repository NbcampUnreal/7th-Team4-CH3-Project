#include "System/F4GameInstance.h"

void UF4GameInstance::WipeData()
{
}

void UF4GameInstance::SaveData()
{
}

void UF4GameInstance::IncrementPermanentDifficulty()
{
	if (bAllowDifficultyScaling)
	{
		PermanentDifficulty++;
		UE_LOG(LogTemp, Log, TEXT("Permanent Difficulty Increased! New Level: %d"), PermanentDifficulty);
	}
}
