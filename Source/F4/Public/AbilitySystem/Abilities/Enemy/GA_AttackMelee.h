#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Enemy/GA_AttackBase.h"
#include "GA_AttackMelee.generated.h"

UCLASS()
class F4_API UGA_AttackMelee : public UGA_AttackBase
{
	GENERATED_BODY()
public:
	UGA_AttackMelee();
	
	virtual void ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
) override;
	
protected:
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
	
	// Gameplay Event 콜백 (AnimNotify 대신)
	UFUNCTION()
	void OnDamageGameplayEvent(FGameplayEventData EventData);
	// Sphere Trace 수행
	void PerformMeleeTrace();
};
