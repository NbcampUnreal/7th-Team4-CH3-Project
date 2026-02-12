#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_ReloadAmmo.generated.h"

UCLASS()
class F4_API UMMC_ReloadAmmo : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_ReloadAmmo();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition CurrentAmmoDef;
	FGameplayEffectAttributeCaptureDefinition MaxAmmoDef;
};
