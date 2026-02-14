#include "AbilitySystem/Abilities/Combat/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/Character.h"
#include "Items/Weapons/F4Projectile.h"
#include "System/F4GameplayTags.h"

UGA_Fire::UGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(F4GameplayTags::Ability_Combat_Fire);

	// TODO: 달리기 중이라면 끊기
	// CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Movement_Sprint)

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);
	ActivationOwnedTags.AddTag(F4GameplayTags::State_Firing);

	// TODO: cost bullet 추가 필요
}

void UGA_Fire::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
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
		FireMontage,
		1.0f
	);

	if (!PlayMontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Fire::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Fire::OnMontageCancelled);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Fire::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		F4GameplayTags::Event_Montage_Fire
	);

	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_Fire::OnFireGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_Fire::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Fire::OnMontageCompleted()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Fire::OnMontageCancelled()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Fire::OnFireGameplayEvent(FGameplayEventData EventData)
{
	SpawnProjectile();
}

void UGA_Fire::SpawnProjectile()
{
	ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!AvatarCharacter || !AvatarCharacter->GetMesh() || !ProjectileClass)
	{
		return;
	}

	FName RightHandSocketName = TEXT("hand_r");
	FVector Start = AvatarCharacter->GetMesh()->GetSocketLocation(RightHandSocketName);
	FVector Forward = AvatarCharacter->GetActorForwardVector();
	FRotator SpawnRotation = Forward.Rotation();

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContext.AddSourceObject(AvatarCharacter);

	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
	   DamageEffectClass,
	   1.0f,
	   EffectContext
	);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AvatarCharacter;
	SpawnParams.Instigator = AvatarCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform(SpawnRotation, Start);

	AF4Projectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AF4Projectile>(
		ProjectileClass,
		SpawnTransform,
		AvatarCharacter,
		AvatarCharacter,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (SpawnedProjectile)
	{
		if (SpecHandle.IsValid())
		{
			SpawnedProjectile->DamageSpecHandle = SpecHandle;
		}

		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}
