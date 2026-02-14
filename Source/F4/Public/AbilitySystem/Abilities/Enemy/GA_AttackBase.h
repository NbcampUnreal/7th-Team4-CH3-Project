#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_AttackBase.generated.h"

UCLASS()
class F4_API UGA_AttackBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_AttackBase();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData
	) override;
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Attack | Montage")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack | Cooldown")
	TSubclassOf<UGameplayEffect> CooldownEffectClass;
	
	// 적용할 Damage Effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Sphere Trace 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float TraceDistance = 100.0f;

	// Sphere Trace 반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float TraceRadius = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug")
	bool bShowdebug = false;
	
	UFUNCTION()
	void OnMontageCompleted();

	// Montage 취소 콜백
	UFUNCTION()
	void OnMontageCancelled();
	
	// Gameplay Event 콜백 (AnimNotify 대신)
	UFUNCTION()
	void OnDamageGameplayEvent(FGameplayEventData EventData);

	// Sphere Trace 수행
	void PerformMeleeTrace();
};
