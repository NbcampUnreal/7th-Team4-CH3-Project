#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Fire.generated.h"

class AF4Projectile;
class UCameraShakeBase;

UCLASS()
class F4_API UGA_Fire : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Fire();

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
	TSubclassOf<AF4Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
	float RecoilSpread = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire|Montage")
	float MontageRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire | Effects")
	TSubclassOf<UCameraShakeBase> FireCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire|Recoil")
	float VerticalRecoilAmount = -0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire|Recoil")
	float HorizontalRecoilRange = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire|Debug")
	bool bIsDebugMode = false;

	UPROPERTY(BlueprintReadOnly, Category = "Fire|Damage")
	float CachedFinalDamage = 0.0f;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnFireGameplayEvent(FGameplayEventData EventData);

	UFUNCTION()
	void ExecuteTriggerGameplayCue();

	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION()
	void CrosshairRecoil();

	UFUNCTION()
	void ApplyAimRecoil();

private:
	void HandleDryFire();

	UFUNCTION()
	void OnDryFireFinished();
};
