#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "F4GameInstance.generated.h"


UCLASS()
class F4_API UF4GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	void WipeData();
	
	void SaveData();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowDifficultyScaling = true;
	UPROPERTY()
	int32 PermanentDifficulty = 0;
	
	UFUNCTION(BlueprintCallable, Category="F4|Difficulty")
	void IncrementPermanentDifficulty();
};
