#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_EnemyDeath.generated.h"

USTRUCT(BlueprintType)
struct FSoundWeightData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(EditAnywhere)
	float Weight = 1.0f;
	
	
};

class AF4EnemyBase;

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

protected:
	void PlayRandomDeathSound(AActor* Actor);
	
	TObjectPtr<USoundBase> SelectRandomSoundFromPool() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Settings")
	TArray<FSoundWeightData> SoundPool;
	
	UPROPERTY(EditAnywhere)
	float SoundMultiplier = 1.0f;
	
	void HandleDropItem();
	void EnableRagdoll(AF4EnemyBase* Enemy);
};
