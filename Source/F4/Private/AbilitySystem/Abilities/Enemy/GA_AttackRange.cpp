#include "AbilitySystem/Abilities/Enemy/GA_AttackRange.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "Items/Weapons/F4Projectile.h"
#include "Kismet/KismetMathLibrary.h"

UGA_AttackRange::UGA_AttackRange()
{
}

void UGA_AttackRange::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	
	MontageTask->OnCompleted.AddDynamic(this, &UGA_AttackRange::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_AttackRange::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_AttackRange::OnMontageCancelled);
	
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			FGameplayTag::RequestGameplayTag(FName("Event.Montage.DealDamage"))
		);
	
	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UGA_AttackRange::OnFireGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}
}

void UGA_AttackRange::OnFireGameplayEvent(FGameplayEventData EventData)
{
	SpawnEnemyProjectile();
}

void UGA_AttackRange::SpawnEnemyProjectile()
{
	AF4EnemyBase* Enemy = Cast<AF4EnemyBase>(GetAvatarActorFromActorInfo());
	AActor* TargetPlayer = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!Enemy || !TargetPlayer || !ProjectileClass)
	{
		return;
	}
	
	AActor* Weapon = Enemy->EquippedWeapon;
	FName MuzzleSocketName = TEXT("Socket_Muzzle");
	
	UMeshComponent* WeaponMesh = Weapon->FindComponentByClass<UMeshComponent>();

	if (WeaponMesh && WeaponMesh->DoesSocketExist(MuzzleSocketName))
	{
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		
		FVector TargetLoc = TargetPlayer->GetActorLocation();
		TargetLoc.Z += 50.f; 
		FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetLoc);
		
		if (MuzzleFlashEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				MuzzleFlashEffect,
				WeaponMesh,
				MuzzleSocketName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true
			);
		}

		FTransform SpawnTransform(AimRotation, MuzzleLocation);
		AF4Projectile* Projectile = GetWorld()->SpawnActorDeferred<AF4Projectile>(
		   ProjectileClass,
		   SpawnTransform,
		   Enemy,
		   Enemy,
		   ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		if (Projectile)
		{
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("무기에서 머즐 소켓을 찾을수 없습니다."));
	}
}
