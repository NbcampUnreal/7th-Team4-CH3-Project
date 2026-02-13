#include "AbilitySystem/Abilities/Enemy/GA_EnemyDeath.h"
#include "System/F4GameplayTags.h"

UGA_EnemyDeath::UGA_EnemyDeath()
{
	AbilityTags.AddTag(F4GameplayTags::Ability_Death);
	
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Dead);
	
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Attack);
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = F4GameplayTags::Event_Character_Die;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    
	AbilityTriggers.Add(TriggerData);
}

void UGA_EnemyDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// 사망처리 관련 여기서 구현 예정
	// 몽타주 실행
	// 콜리전끄기 등등...
	
	GetAvatarActorFromActorInfo()->Destroy();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
