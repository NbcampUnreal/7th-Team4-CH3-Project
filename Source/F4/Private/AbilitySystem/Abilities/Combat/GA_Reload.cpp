#include "AbilitySystem/Abilities/Combat/GA_Reload.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "System/F4GameplayTags.h"

UGA_Reload::UGA_Reload()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(F4GameplayTags::Ability_Combat_Reload);

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Reloading);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);

	ActivationOwnedTags.AddTag(F4GameplayTags::State_Reloading);

	// TODO: cost - ammo(총알) 인벤토리에 있어야 장전가능 일단은 그냥 총알 무한으로 장전가능하게
	// TODO: 이미 탄창이 MAX라면 실행안되게
}

void UGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ReloadMontage,
		1.0f
	);

	if (!PlayMontageTask )
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Reload::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Reload::OnMontageCancelled);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Reload::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		F4GameplayTags::Event_Montage_Reload
	);

	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_Reload::OnReloadGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Reload::OnMontageCompleted()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Reload::OnMontageCancelled()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Reload::OnReloadGameplayEvent(FGameplayEventData EventData)
{
	PerformReload();
}

void UGA_Reload::PerformReload()
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC || !ReloadEffectClass) return;

	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		ReloadEffectClass,
		1.0f,
		EffectContext
	);

	if (SpecHandle.IsValid())
	{
		SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		UE_LOG(LogTemp, Warning, TEXT("Apply Reload GE."));
	}
}
