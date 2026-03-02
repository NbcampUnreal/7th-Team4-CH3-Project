#pragma once

#include "CoreMinimal.h"
#include "GA_ConsumableBase.h"
#include "GA_UsePoition.generated.h"

UCLASS()
class F4_API UGA_UsePoition : public UGA_ConsumableBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnConsumeActivated(UF4ItemInstance* Item) override;
};
