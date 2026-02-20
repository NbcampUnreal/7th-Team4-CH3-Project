#include "AbilitySystem/Abilities/Combat/GA_WeaponHit.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "System/F4GameplayTags.h"


UGA_WeaponHit::UGA_WeaponHit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = F4GameplayTags::Event_Hit_Damage;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UGA_WeaponHit::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !TriggerEventData->Target)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TriggerEventData->Target);
	if (!TargetASC || !DamageEffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	if (TriggerEventData->TargetData.Num() > 0)
	{
		ContextHandle.AddHitResult(*TriggerEventData->TargetData.Get(0)->GetHitResult(), true);
	}

	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		DamageEffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
