#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_EnemyDeath.generated.h"

UCLASS()
class F4_API UGA_EnemyDeath : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_EnemyDeath();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData
	) override;
};
