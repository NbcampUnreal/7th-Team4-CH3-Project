#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_SwitchWeapon.generated.h"

UCLASS()
class F4_API UGA_SwitchWeapon : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_SwitchWeapon();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UAnimMontage> SwitchMontage;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnSwitchingGameplayEvent(FGameplayEventData EventData);

private:
	int32 CachedTargetSlotIndex = 0;

	void PerformSwitch();
};
