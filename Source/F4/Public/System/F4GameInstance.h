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
	
};
