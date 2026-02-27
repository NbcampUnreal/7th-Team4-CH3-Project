#include "AbilitySystem/Calculations/MMC_EnemyMaxHealth.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "System/F4GameState.h"

UMMC_EnemyMaxHealth::UMMC_EnemyMaxHealth()
{
	MaxHealthDef.AttributeToCapture = UF4AttributeSetCharacter::GetMaxHealthAttribute();
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHealthDef.bSnapshot = false; // 실시간 반영을 위해 false
	
	RelevantAttributesToCapture.Add(MaxHealthDef);
}

float UMMC_EnemyMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float BaseMaxHealth = 0.f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluationParameters, BaseMaxHealth);
	BaseMaxHealth = FMath::Max<float>(BaseMaxHealth, 0.f);
	
	UWorld* World = Spec.GetContext().GetOriginalInstigator()
					? Spec.GetContext().GetOriginalInstigator()->GetWorld()
					: nullptr;
	int32 Phase = 0;
	if (World)
	{
		if (AF4GameState* GameState = World->GetGameState<AF4GameState>())
		{
			Phase = FMath::Max(GameState->DifficultyPhase - 1, 1);
			
		}
	}
	return BaseMaxHealth + (static_cast<float>(Phase) * 50.f);
}
