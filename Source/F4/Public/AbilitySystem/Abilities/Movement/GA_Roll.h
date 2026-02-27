#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Roll.generated.h"


UCLASS()
class F4_API UGA_Roll : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Roll();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* RollMontage; 
};
