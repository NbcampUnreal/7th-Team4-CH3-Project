#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_WeaponHit.generated.h"

UCLASS()
class F4_API UGA_WeaponHit : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_WeaponHit();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DamageEffect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
