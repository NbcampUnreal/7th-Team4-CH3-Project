#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Death.generated.h"

UCLASS()
class F4_API UGA_Death : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Death();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UFUNCTION()
	void OnEndDelay(); 
	
};
