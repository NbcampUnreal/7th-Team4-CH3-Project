#include "AbilitySystem/Abilities/Interaction/GA_Pickup.h"

#include "AbilitySystemComponent.h"
#include "Items/F4ItemDataAsset.h"
#include "System/F4GameplayTags.h"

UGA_Pickup::UGA_Pickup()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer CurrentTags = GetAssetTags();
	CurrentTags.AddTag(F4GameplayTags::Ability_Interaction_Pickup);
	SetAssetTags(CurrentTags);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = F4GameplayTags::Event_Interaction_Pickup;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UGA_Pickup::ActivateAbility(
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
	
	const UF4ItemDataAsset* PickedItem = Cast<UF4ItemDataAsset>(TriggerEventData->OptionalObject);
	if (!PickedItem)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (PickedItem->ItemType == EF4ItemType::Weapon)
	{
		// TODO: 인벤토리 검사 후 바로 장착할 수 있는 장비 칸이 없으면 인벤토리에 추가
		
		// 바로 장착할 수 있는 장비 칸이 있으면 착용 이벤트 전달  
		GetAbilitySystemComponentFromActorInfo()->
		HandleGameplayEvent(F4GameplayTags::Event_Combat_Equip, TriggerEventData);
	}
	else if (PickedItem->ItemType == EF4ItemType::Consumable || PickedItem->ItemType == EF4ItemType::Ammo)
	{
		// TODO: 소모품이거나 총알이면 인벤토리에 추가
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
