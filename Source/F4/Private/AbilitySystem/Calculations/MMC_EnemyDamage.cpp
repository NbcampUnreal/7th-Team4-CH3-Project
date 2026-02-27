#include "AbilitySystem/Calculations/MMC_EnemyDamage.h"

#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "System/F4GameState.h"

UMMC_EnemyDamage::UMMC_EnemyDamage()
{
	AttackDef.AttributeToCapture = UF4AttributeSetCharacter::GetATKAttribute();
	AttackDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(AttackDef);
}

float UMMC_EnemyDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float AttackPower = 0.f;
	GetCapturedAttributeMagnitude(AttackDef, Spec, EvaluateParameters, AttackPower);
	
	float DifficultyMultiplier = 1.f;
	
	UWorld* World = Spec.GetContext().GetOriginalInstigator()
					? Spec.GetContext().GetOriginalInstigator()->GetWorld()
					: nullptr;
	if (World)
	{
		if (AF4GameState* GameState = World->GetGameState<AF4GameState>())
		{
			int32 Phase = FMath::Max(GameState->DifficultyPhase, 1);
			DifficultyMultiplier = 1.0f + (Phase - 1) * 0.2f;
		}
	}
	float FinalDamage = AttackPower * DifficultyMultiplier;
	FinalDamage = FMath::FloorToFloat(FinalDamage);
	
	return -FMath::Max(FinalDamage, 0.f);
}
