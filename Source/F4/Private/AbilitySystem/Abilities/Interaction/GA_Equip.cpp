#include "AbilitySystem/Abilities/Interaction/GA_Equip.h"

#include "Characters/Player/F4PlayerCharacter.h"
#include "Items/Weapons/F4WeaponDataAsset.h"
#include "System/F4GameplayTags.h"

UGA_Equip::UGA_Equip()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer CurrentTags = GetAssetTags();
	CurrentTags.AddTag(F4GameplayTags::Ability_Combat_Equip);
	SetAssetTags(CurrentTags);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = F4GameplayTags::Event_Combat_Equip;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UGA_Equip::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	const UF4WeaponDataAsset* WeaponData = Cast<UF4WeaponDataAsset>(TriggerEventData->OptionalObject);
	AF4PlayerCharacter* Character = Cast<AF4PlayerCharacter>(GetAvatarActorFromActorInfo());
	
	if (!WeaponData || !Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// TODO: 착용함수를 GA_Equip에 할 지 플레이어에서 할 지

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
