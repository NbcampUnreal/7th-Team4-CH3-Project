#include "AbilitySystem/Abilities/Combat/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "AbilitySystem/Attributes/F4AttributeSetWeapon.h"
#include "GameFramework/Character.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Firearm.h"
#include "Inventory/F4ItemInstance.h"
#include "Items/Weapons/F4Projectile.h"
#include "Items/Weapons/F4WeaponActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/F4GameplayTags.h"
#include "UI/F4HUD.h"



UGA_Fire::UGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Combat_Fire));

	// TODO: 달리기 중이라면 끊기
	// CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Movement_Sprint)

	ActivationRequiredTags.AddTag(F4GameplayTags::State_Aiming);

	ActivationBlockedTags.AddTag(F4GameplayTags::State_Firing);
	ActivationBlockedTags.AddTag(F4GameplayTags::State_Switching_Weapon);
	ActivationBlockedTags.AddTag(F4GameplayTags::Character_State_Rolling);
	ActivationBlockedTags.AddTag(F4GameplayTags::Character_State_HurricaneKicking);
	ActivationBlockedTags.AddTag(F4GameplayTags::Character_State_Sprinting);

	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_Invisible);
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Combat_Reload);

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
		// TODO: 총알 빈 소리
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CachedFinalDamage = 0.0f;
	float DynamicMontageRate = MontageRate;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		if (ASC->HasAttributeSetForAttribute(UF4AttributeSetWeapon::GetBaseDamageAttribute()))
		{
			CachedFinalDamage = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetBaseDamageAttribute());
		}

		if (ASC->HasAttributeSetForAttribute(UF4AttributeSetCharacter::GetATKAttribute()))
		{
			float CharacterATK = ASC->GetNumericAttribute(UF4AttributeSetCharacter::GetATKAttribute());
			CachedFinalDamage *= CharacterATK;
		}

		if (ASC->HasAttributeSetForAttribute(UF4AttributeSetWeapon::GetFireRateAttribute()))
		{
			DynamicMontageRate = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetFireRateAttribute());
		}
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		FireMontage,
		DynamicMontageRate
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

	UAbilityTask_WaitGameplayEvent* WaitFireEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		F4GameplayTags::Event_Montage_Fire
	);

	if (WaitFireEventTask)
	{
		WaitFireEventTask->EventReceived.AddDynamic(this, &UGA_Fire::OnFireGameplayEvent);
		WaitFireEventTask->ReadyForActivation();
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
	
	CrosshairRecoil();
	ApplyAimRecoil();

	ExecuteTriggerGameplayCue();
}

void UGA_Fire::ExecuteTriggerGameplayCue()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	AF4PlayerCharacter* PlayerChar = Cast<AF4PlayerCharacter>(GetAvatarActorFromActorInfo());

	if (!ASC || !PlayerChar)
	{
		return;
	}

	UF4EquipmentComponent* EquipComp = PlayerChar->FindComponentByClass<UF4EquipmentComponent>();
	AF4WeaponActor* ActiveWeapon = EquipComp ? EquipComp->GetActiveWeaponActor() : nullptr;

	if (ActiveWeapon)
	{
		FGameplayCueParameters Params;
		Params.Instigator = PlayerChar;

		Params.Location = ActiveWeapon->GetMuzzleTransform().GetLocation();
		Params.Normal = ActiveWeapon->GetMuzzleTransform().GetRotation().GetForwardVector();

		Params.EffectCauser = ActiveWeapon;

		ASC->ExecuteGameplayCue(F4GameplayTags::GameplayCue_Weapon_Triggered, Params);
	}
}

void UGA_Fire::SpawnProjectile()
{
	AF4PlayerCharacter* AvatarCharacter = Cast<AF4PlayerCharacter>(GetAvatarActorFromActorInfo());
    if (!AvatarCharacter || !AvatarCharacter->GetMesh() || !ProjectileClass)
    {
       return;
    }

    UF4EquipmentComponent* EquipmentComp = AvatarCharacter->FindComponentByClass<UF4EquipmentComponent>();
    AF4WeaponActor* ActiveWeapon = EquipmentComp ? EquipmentComp->GetActiveWeaponActor() : nullptr;

    if (!ActiveWeapon)
    {
       return;
    }

    FTransform MuzzleTransform = ActiveWeapon->GetMuzzleTransform();
    FVector MuzzleLocation = MuzzleTransform.GetLocation();

    FVector CameraLocation;
    FRotator CameraRotation;
    AvatarCharacter->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 50000.f);
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(AvatarCharacter);

    GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_GameTraceChannel1, TraceParams);
    if (HitResult.bBlockingHit && HitResult.GetActor() && bIsDebugMode)
    {
       GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("타겟 액터: %s"), *HitResult.GetActor()->GetName()));
    }
    FVector TargetLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

    FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetLocation);
    FTransform SpawnTransform(AimRotation, MuzzleLocation);

    AF4Projectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AF4Projectile>(
       ProjectileClass,
       SpawnTransform,
       AvatarCharacter,
       AvatarCharacter,
       ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    );

    if (bIsDebugMode)
    {
       DrawDebugLine(GetWorld(), CameraLocation, TargetLocation, FColor::Red, false, 3.0f, 0, 0.2f);
       DrawDebugLine(GetWorld(), MuzzleLocation, TargetLocation, FColor::Green, false, 3.0f, 0, 1.5f);
       DrawDebugSphere(GetWorld(), TargetLocation, 15.0f, 12, FColor::Yellow, false, 3.0f);
    }

    if (SpawnedProjectile)
    {
       SpawnedProjectile->DamagePayload = CachedFinalDamage;
       SpawnedProjectile->FinishSpawning(SpawnTransform);
    }
}

void UGA_Fire::CrosshairRecoil()
{
	// 크로스헤어 벌어지게 하는 함수 
	if (AF4PlayerCharacter* PlayerChar = Cast<AF4PlayerCharacter>(GetOwningActorFromActorInfo()))
	{
		if (PlayerChar->HUDWidget) 
		{
			PlayerChar->HUDWidget->AddRecoilImpulse(20.f);
		}

		if (APlayerController* PlayerController = Cast<APlayerController>(PlayerChar->GetController()))
		{
			if (FireCameraShakeClass)
			{
				PlayerController->ClientStartCameraShake(FireCameraShakeClass);
			}
		}
	}
}

void UGA_Fire::ApplyAimRecoil()
{
	APawn* OwningPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!OwningPawn) return;

	APlayerController* PlayerController = Cast<APlayerController>(OwningPawn->GetController());
	if (!PlayerController) return;

	PlayerController->AddPitchInput(VerticalRecoilAmount);

	float RandomYaw = FMath::FRandRange(-HorizontalRecoilRange, HorizontalRecoilRange);
	PlayerController->AddYawInput(RandomYaw);
}
