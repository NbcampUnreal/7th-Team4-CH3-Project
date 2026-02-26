#pragma once

#include "CoreMinimal.h"
#include "GA_AttackBase.h"
#include "GA_AttackRange.generated.h"

class AF4Projectile;

UCLASS()
class F4_API UGA_AttackRange : public UGA_AttackBase
{
	GENERATED_BODY()
public:
	UGA_AttackRange();
	
	virtual void ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
	TSubclassOf<AF4Projectile> ProjectileClass;
	
	UFUNCTION()
	void OnFireGameplayEvent(FGameplayEventData EventData);
	
	void SpawnEnemyProjectile();
};
