#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Potion_Invincible.generated.h"

class UMaterialInterface;
class UGameplayEffect;

UCLASS()
class F4_API UGA_Potion_Invincible : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Potion_Invincible();

protected:
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
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion|Effect")
	TSubclassOf<UGameplayEffect> InvincibilityEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion|Visual")
	TObjectPtr<UMaterialInterface> TransparentMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion|Time")
	float Duration = 5.0f;
	
protected:
	UFUNCTION()
	void OnDurationEnded();
	
	UFUNCTION()
	void OnActionDetected();
	
private:
	FActiveGameplayEffectHandle ActiveEffectHandle;
	
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OriginalMaterials;
};
