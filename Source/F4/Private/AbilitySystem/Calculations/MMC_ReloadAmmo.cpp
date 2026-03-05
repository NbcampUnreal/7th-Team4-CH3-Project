#include "AbilitySystem/Calculations/MMC_ReloadAmmo.h"

#include "AbilitySystem/Attributes/F4AttributeSetWeapon.h"

UMMC_ReloadAmmo::UMMC_ReloadAmmo()
{
	CurrentAmmoDef.AttributeToCapture = UF4AttributeSetWeapon::GetCurrentAmmoAttribute();
	CurrentAmmoDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CurrentAmmoDef.bSnapshot = false;

	MaxAmmoDef.AttributeToCapture = UF4AttributeSetWeapon::GetMaxAmmoAttribute();
	MaxAmmoDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxAmmoDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CurrentAmmoDef);
	RelevantAttributesToCapture.Add(MaxAmmoDef);
}

float UMMC_ReloadAmmo::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// TODO: 현재는 무한 탄약 -> 소지한 총알이 10개밖에 없으면 10개만 + 하도록 변경해야 함

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float CurrentAmmo = 0.f;
	GetCapturedAttributeMagnitude(CurrentAmmoDef, Spec, EvaluationParameters, CurrentAmmo);

	float MaxAmmo = 0.f;
	GetCapturedAttributeMagnitude(MaxAmmoDef, Spec, EvaluationParameters, MaxAmmo);

	float ReloadAmount = MaxAmmo - CurrentAmmo;

	return FMath::Max(0.f, ReloadAmount);
}
