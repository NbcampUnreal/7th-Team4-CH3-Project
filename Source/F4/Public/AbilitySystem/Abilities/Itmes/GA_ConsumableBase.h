#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ConsumableBase.generated.h"

class UF4ItemInstance;

UCLASS(Abstract)
class F4_API UGA_ConsumableBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ConsumableBase();

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

	// 자식 클래스에서 구현: 아이템 효과 적용 + 어빌리티 태스크 셋업
	virtual void OnConsumeActivated(UF4ItemInstance* Item) {}

	// 자식 클래스에서 구현: 어빌리티 종료 시 정리 (GE 제거, 시각 효과 복구 등)
	virtual void OnConsumeEnded() {}
};
