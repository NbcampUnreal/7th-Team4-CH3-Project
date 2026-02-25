#pragma once

#include "CoreMinimal.h"
#include "GA_AttackBase.h"
#include "GA_AttackRange.generated.h"

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
	// 원거리 공격에 필요한것들 추가에정
};
