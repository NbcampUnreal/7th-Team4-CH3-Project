#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_HurricaneKick.generated.h"

UCLASS()
class F4_API UGA_HurricaneKick : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_HurricaneKick();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) override;

	void ToggleTimers(bool bStart);

	void DetectEnemies();

	UFUNCTION()
	void OnCompleted();

	UFUNCTION()
	void OnStaminaExhausted(FGameplayEventData Data);

	void StopHurricaneKick();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> SpeedEffect;

	FActiveGameplayEffectHandle SpeedEffectHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> CostEffect;

	FActiveGameplayEffectHandle CostEffectHandle;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* HurricaneKickMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float ActiveDuration = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	FName StopSectionName;

protected:
	FTimerHandle DurationTimerHandle;

	FTimerHandle HitTimerHandle;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
	FName TraceStartBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
	float TraceRadius = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
	float TraceDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
	bool bShowDebug = true;
};
