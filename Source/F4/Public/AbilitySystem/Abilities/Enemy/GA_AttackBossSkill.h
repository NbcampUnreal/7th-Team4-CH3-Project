#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Enemy/GA_AttackBase.h"
#include "GA_AttackBossSkill.generated.h"

class AF4Projectile;

UCLASS()
class F4_API UGA_AttackBossSkill : public UGA_AttackBase
{
	GENERATED_BODY()
public:
	UGA_AttackBossSkill();
	
	virtual void ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
) override;
	
	// 블루프린트 노티파이에서 호출할 수 있도록 BlueprintCallable 추가
	UFUNCTION(BlueprintCallable, Category = "F4 | Ability")
	void SpawnCross();

	UFUNCTION(BlueprintCallable, Category = "F4 | Ability")
	void SpawnXShape();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
	TSubclassOf<AF4Projectile> ProjectileClass;
	
	UFUNCTION()
	void OnFireGameplayEvent(FGameplayEventData EventData);
	
	void ManageSpawnProjectile(float InStartOffset);
	
	FGameplayTagContainer* GetCooldownTag() const;
};
