#include "AbilitySystem/Abilities/Enemy/GA_AttackBase.h"
#include"AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"

UGA_AttackBase::UGA_AttackBase()
{
	// 1. 이 어빌리티의 이름표
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Attack));

	// 2. 실행 중일 때 캐릭터에게 부여될 태그 (다른 곳에서 상태 확인용)
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Attacking);

	// 3. 실행을 막는 조건들
	ActivationBlockedTags.AddTag(F4GameplayTags::Ability_Death);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Attacking);
}

void UGA_AttackBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_AttackBase::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
