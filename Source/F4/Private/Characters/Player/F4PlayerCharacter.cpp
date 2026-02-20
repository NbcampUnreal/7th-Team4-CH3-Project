// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/F4PlayerCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/ConsumableItems/F4ConsumableDataAsset.h"
#include "Items/Weapons/F4WeaponActor.h"
#include "Items/Weapons/F4WeaponDataAsset.h"
#include "System/F4GameplayTags.h"


AF4PlayerCharacter::AF4PlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -10.0f, 0.0f));
	CameraBoom->TargetArmLength = 110.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AF4PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AF4PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->ViewPitchMin = -90.0f;
			PC->PlayerCameraManager->ViewPitchMax = 30.0f;

			// PC->PlayerCameraManager->ViewYawMin = -60.0f;
			// PC->PlayerCameraManager->ViewYawMax = 60.0f;
		}
	}
}

void AF4PlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (LookAction)
		{
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Look);
		}

		if (ZoomAction)
		{
			EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Zoom);
		}

		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Move);
		}

		if (JumpAction)
		{
			EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Jump);
		}

		if (RollAction)
		{
			EIC->BindAction(RollAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Roll);
		}

		if (InteractAction)
		{
			EIC->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Interact);
		}

		if (SprintAction)
		{
			EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AF4PlayerCharacter::ToggleSprint);
		}

		// if (CrouchAction)
		// {
		// 	  EIC->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Crouch);
		// }

		if (AttackAction)
		{
			EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Attack);
		}

		if (FireAction)
		{
			EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::OnFire);
		}

		if (AimAction)
		{
			EIC->BindAction(AimAction, ETriggerEvent::Started, this, &AF4PlayerCharacter::OnAimStarted);
			EIC->BindAction(AimAction, ETriggerEvent::Completed, this, &AF4PlayerCharacter::OnAimReleased);
		}

		if (ReloadAction)
		{
			EIC->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::OnReload);
		}
	}
}

FText AF4PlayerCharacter::GetInteractionText() const
{
	return FText::FromString(TEXT("Player"));
}

void AF4PlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	const FRotator Rotaion = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotaion.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void AF4PlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void AF4PlayerCharacter::Zoom(const FInputActionValue& Value)
{
	float ZoomFactor = Value.Get<float>();

	if (GetController() != nullptr)
	{
		float TargetLength = CameraBoom->TargetArmLength - ZoomFactor * ZoomStep;
		TargetLength = FMath::Clamp(TargetLength, MinTargetArmLength, MaxTargetArmLength);

		CameraBoom->TargetArmLength = TargetLength;
	}
}

void AF4PlayerCharacter::Jump()
{
	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Movement_Jump);

	ASC->TryActivateAbilitiesByTag(Container);
}

void AF4PlayerCharacter::Roll()
{
	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Movement_Roll);

	ASC->TryActivateAbilitiesByTag(Container);
}

// void AF4PlayerCharacter::Crouch()
// {
// 	FGameplayTagContainer Container;
// 	Container.AddTag(F4GameplayTags::Ability_Movement_Crouch);
//
// 	ASC->TryActivateAbilitiesByTag(Container);
// }

void AF4PlayerCharacter::ToggleSprint()
{
	FGameplayTag StateTag = F4GameplayTags::Character_State_Sprinting;

	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Movement_Sprint);

	if (ASC->HasMatchingGameplayTag(StateTag))
	{
		// Stop Sprinting

		ASC->CancelAbilities(&Container);
	}
	else
	{
		// Start Sprinting
		ASC->TryActivateAbilitiesByTag(Container);
	}
}


void AF4PlayerCharacter::Attack()
{
}

void AF4PlayerCharacter::Interact()
{
	// Interact -> GA_Interact(Trace) -> DoInteract(Interact)

	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Interaction_Interact);

	ASC->TryActivateAbilitiesByTag(Container);
}

void AF4PlayerCharacter::OnFire()
{
	if (!ASC)
	{
		return;
	}

	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Combat_Fire);

	ASC->TryActivateAbilitiesByTag(Container);
}

void AF4PlayerCharacter::OnAimStarted()
{
	if (!ASC)
	{
		return;
	}

	// TODO: input.ID 처리 방식 고려
	ASC->AbilityLocalInputPressed(30);

	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Combat_Aim);
	ASC->TryActivateAbilitiesByTag(Container);
}

void AF4PlayerCharacter::OnAimReleased()
{
	if (!ASC)
	{
		return;
	}

	// TODO: input.ID 처리 방식 고려
	ASC->AbilityLocalInputReleased(30);
}

void AF4PlayerCharacter::OnReload()
{
	if (!ASC)
	{
		return;
	}

	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Combat_Reload);

	ASC->TryActivateAbilitiesByTag(Container);
}

void AF4PlayerCharacter::ProcessItemPickup(const UF4ItemDataAsset* PickupItemData)
{
	if (!PickupItemData) return;
	
	switch (PickupItemData->ItemType)
	{
		case EF4ItemType::Weapon:
		{
			const UF4WeaponDataAsset* WeaponData = Cast<UF4WeaponDataAsset>(PickupItemData);
			EquipWeapon(WeaponData);
			break;
		}
		case EF4ItemType::Consumable:
		{
			const UF4ConsumableDataAsset* ConsumableData = Cast<UF4ConsumableDataAsset>(PickupItemData);
			// TODO: ConsumeItem(ConsumableData);
			break;
		}
	}
}

void AF4PlayerCharacter::EquipWeapon(const UF4WeaponDataAsset* NewWeaponData)
{
	if (!NewWeaponData) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	AF4WeaponActor* NewWeapon = GetWorld()->SpawnActor<AF4WeaponActor>(
		AF4WeaponActor::StaticClass(),
		GetActorTransform(),
		SpawnParams
	);

	// 무기 초기화 및 소켓 부착
	if (NewWeapon)
	{
		NewWeapon->InitializeWeapon(NewWeaponData);

		NewWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			TEXT("WeaponSocket")
		);

		CurrentWeapon = NewWeapon;

		GrantWeaponAbilities(NewWeaponData);
	}
}

void AF4PlayerCharacter::GrantWeaponAbilities(const UF4WeaponDataAsset* WeaponData)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();


	if (!WeaponData || !AbilitySystemComponent) return;

	for (const FWeaponAbilityConfig& AbilityConfig : WeaponData->Abilities)
	{
		if (AbilityConfig.AbilityClass)
		{
			// Spec 생성
			FGameplayAbilitySpec Spec(AbilityConfig.AbilityClass, 1);

			// 입력 태그를 Spec에 동적 태그로 추가
			Spec.GetDynamicSpecSourceTags().AddTag(AbilityConfig.InputTag);

			AbilitySystemComponent->GiveAbility(Spec);
		}
	}
}

void AF4PlayerCharacter::DoInteract(AActor* Interactor)
{
}
