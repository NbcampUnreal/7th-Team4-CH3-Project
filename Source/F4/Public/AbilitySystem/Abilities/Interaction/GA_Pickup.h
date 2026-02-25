#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Pickup.generated.h"

UCLASS()
class F4_API UGA_Pickup : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Pickup(); 
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
