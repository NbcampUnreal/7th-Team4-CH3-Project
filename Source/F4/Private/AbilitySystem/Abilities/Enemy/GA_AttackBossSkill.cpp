#include "AbilitySystem/Abilities/Enemy/GA_AttackBossSkill.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Items/Weapons/F4Projectile.h"

UGA_AttackBossSkill::UGA_AttackBossSkill()
{
}

void UGA_AttackBossSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
	if (!CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true,false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask = 
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, 
		NAME_None,
		AttackMontage,
		MontageSpeed
		);
	
	if (!MontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	MontageTask->OnCompleted.AddDynamic(this, &UGA_AttackBossSkill::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_AttackBossSkill::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_AttackBossSkill::OnMontageCancelled);
	
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask =
	   UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		  this,
		  FGameplayTag::RequestGameplayTag(FName("Event.Boss")),
		  nullptr,
		  false, 
		  false
	   );
	
	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_AttackBossSkill::OnFireGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_AttackBossSkill::SpawnCross()
{
	ManageSpawnProjectile(0.f);
}

void UGA_AttackBossSkill::SpawnXShape()
{
	ManageSpawnProjectile(45.f);
}

void UGA_AttackBossSkill::OnFireGameplayEvent(FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Event Received: %s"), *EventData.EventTag.ToString());
	
	FGameplayTag CrossTag = FGameplayTag::RequestGameplayTag(FName("Event.Boss.Cross"));
	FGameplayTag XShapeTag = FGameplayTag::RequestGameplayTag(FName("Event.Boss.XShape"));

	if (EventData.EventTag.MatchesTag(CrossTag))
	{
		SpawnCross();
	}
	else if (EventData.EventTag.MatchesTag(XShapeTag))
	{
		SpawnXShape();
	}
}

void UGA_AttackBossSkill::ManageSpawnProjectile(float InStartOffset)
{
	AActor* OwningActor = GetAvatarActorFromActorInfo();
	if (!OwningActor || !ProjectileClass || !GetWorld()) return;
	
	FVector BaseLocation = OwningActor->GetActorLocation() + FVector(0.f, 0.f, 0.f);
	float BaseYaw = OwningActor->GetActorRotation().Yaw;
	
	const float SpawnOffsetDist = 100.f; 

	for (int32 i = 0; i < 4; ++i)
	{
		float TargetYaw = BaseYaw + InStartOffset + (i * 90.f);
		FRotator SpawnRotation = FRotator(0.f, TargetYaw, 0.f);
		
		FVector FinalSpawnLocation = BaseLocation + (SpawnRotation.Vector() * SpawnOffsetDist);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwningActor;
		SpawnParams.Instigator = Cast<APawn>(OwningActor);
		
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
		GetWorld()->SpawnActor<AF4Projectile>(ProjectileClass, FinalSpawnLocation, SpawnRotation, SpawnParams);
	}
}

FGameplayTagContainer* UGA_AttackBossSkill::GetCooldownTag() const
{
	static FGameplayTagContainer CooldownTags;
	CooldownTags.Reset();
	CooldownTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Attack")));
	return &CooldownTags;
}
