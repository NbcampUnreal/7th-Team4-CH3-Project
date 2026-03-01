#include "AbilitySystem/Abilities/Itmes/GA_ConsumableBase.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemInstance.h"
#include "System/F4GameplayTags.h"

UGA_ConsumableBase::UGA_ConsumableBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = F4GameplayTags::Event_Trigger_UseConsumable;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UGA_ConsumableBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 이 이벤트가 내 아이템을 대상으로 한 것인지 확인
	// Spec 생성 시 SourceObject = ItemInstance 로 저장되어 있음
	UF4ItemInstance* TriggeredItem = TriggerEventData
		? const_cast<UF4ItemInstance*>(Cast<UF4ItemInstance>(TriggerEventData->OptionalObject))
		: nullptr;
	UF4ItemInstance* MyItem = Cast<UF4ItemInstance>(GetCurrentSourceObject());

	if (TriggeredItem != MyItem)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	OnConsumeActivated(MyItem);

	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UF4InventoryComponent* InvComp = Character->FindComponentByClass<UF4InventoryComponent>())
		{
			InvComp->ConsumeItem(MyItem, 1);
		}
	}
}

void UGA_ConsumableBase::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	OnConsumeEnded();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
