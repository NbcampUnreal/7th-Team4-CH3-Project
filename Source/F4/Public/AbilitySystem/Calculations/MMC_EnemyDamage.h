#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_EnemyDamage.generated.h"

UCLASS()
class F4_API UMMC_EnemyDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_EnemyDamage();
	
	virtual float  CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:
	FGameplayEffectAttributeCaptureDefinition AttackDef;
};
