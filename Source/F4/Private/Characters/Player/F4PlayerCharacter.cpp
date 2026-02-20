// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/F4PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Input/F4InputComponent.h"


// #include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"

#include "Items/Weapons/F4WeaponActor.h"
#include "Items/Weapons/F4WeaponDataAsset.h"




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

void AF4PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UF4AbilitySystemComponent* F4ASC = Cast<UF4AbilitySystemComponent>(GetAbilitySystemComponent());
	
	if (F4ASC)
	{
		F4ASC->ProcessAbilityInput(DeltaTime,false);		
	}
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
	
	UF4InputComponent* F4EIC = Cast<UF4InputComponent>(PlayerInputComponent);
	if (!F4EIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Input Component is NULL"));
		return;
	}
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer())
	)
	{
		Subsystem->AddMappingContext(DefaultIMC, 0);	
	}
	
	if (InputConfig)
	{
		// Native Input Actions 
		F4EIC->BindNativeAction(
			InputConfig,
			F4GameplayTags::InputTag_Move,
			ETriggerEvent::Triggered,
			this,
			&ThisClass::Input_Move,
			true
		);
		
		F4EIC->BindNativeAction(
			InputConfig,
			F4GameplayTags::InputTag_Look,
			ETriggerEvent::Triggered,
			this,
			&ThisClass::Input_Look,
			true
		);
		
		F4EIC->BindNativeAction(
			InputConfig, 
			F4GameplayTags::InputTag_Zoom,
			ETriggerEvent::Triggered,
			this,
			&ThisClass::Input_Zoom,
			true
		);
		
		// Ability Input Actions 
		TArray<uint32> BindHandles;
		F4EIC->BindAbilityActions(
			InputConfig,
			this, 
			&ThisClass::Input_AbilityPressed,
			&ThisClass::Input_AbilityReleased,
			BindHandles
		);
	}
	
	// if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	// {
	// 	if (LookAction)
	// 	{
	// 		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Look);
	// 	}
	//
	// 	if (ZoomAction)
	// 	{
	// 		EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Zoom);
	// 	}
	//
	// 	if (MoveAction)
	// 	{
	// 		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Move);
	// 	}
	//
	// 	if (JumpAction)
	// 	{
	// 		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Jump);
	// 	}
	//
	// 	if (RollAction)
	// 	{
	// 		EIC->BindAction(RollAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Roll);
	// 	}
	//
	// 	if (InteractAction)
	// 	{
	// 		EIC->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Interact);
	// 	}
	//
	// 	if (SprintAction)
	// 	{
	// 		EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AF4PlayerCharacter::ToggleSprint);
	// 	}
	//
	// 	// if (CrouchAction)
	// 	// {
	// 	// 	  EIC->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Crouch);
	// 	// }
	//
	// 	if (AttackAction)
	// 	{
	// 		EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Attack);
	// 	}
	//
	// 	if (FireAction)
	// 	{
	// 		EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::OnFire);
	// 	}
	//
	// 	if (AimAction)
	// 	{
	// 		EIC->BindAction(AimAction, ETriggerEvent::Started, this, &AF4PlayerCharacter::OnAimStarted);
	// 		EIC->BindAction(AimAction, ETriggerEvent::Completed, this, &AF4PlayerCharacter::OnAimReleased);
	// 	}
	//
	// 	if (ReloadAction)
	// 	{
	// 		EIC->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::OnReload);
	// 	}
	// }
}

FText AF4PlayerCharacter::GetInteractionText() const
{
	return FText::FromString(TEXT("Player"));
}

void AF4PlayerCharacter::Input_Move(const FInputActionValue& Value)
{
	APlayerController* PC = CastChecked<APlayerController>(GetController());
	if (!PC) return;
	
	const FVector2D MoveValue = Value.Get<FVector2D>();
	const FRotator MoveRotation(0.f, PC->GetControlRotation().Yaw, 0.f);
	
	if (MoveValue.X != 0.f)
	{
		const FVector MoveDirection = MoveRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MoveDirection, MoveValue.X); 
	}
	
	if (MoveValue.Y != 0.f)
	{
		const FVector MoveDirection = MoveRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MoveDirection, MoveValue.Y); 
	}
}

void AF4PlayerCharacter::Input_Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	
	if (!LookValue.IsNearlyZero())
	{
		AddControllerYawInput(LookValue.X);
		AddControllerPitchInput(LookValue.Y);
	}
}

void AF4PlayerCharacter::Input_Zoom(const FInputActionValue& Value)
{
	float ZoomFactor = Value.Get<float>();

	if (GetController() != nullptr)
	{
		float TargetLength = CameraBoom->TargetArmLength - ZoomFactor * ZoomStep;
		TargetLength = FMath::Clamp(TargetLength, MinTargetArmLength, MaxTargetArmLength);

		CameraBoom->TargetArmLength = TargetLength;
	}
}

void AF4PlayerCharacter::Input_AbilityPressed(const FGameplayTag InputTag)
{
	if (UF4AbilitySystemComponent* F4ASC = Cast<UF4AbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		F4ASC->AbilityInputTagPressed(InputTag);
		// if (InputTag == F4GameplayTags::Ability_Combat_Aim)
		// {
		// 	F4ASC->AbilityLocalInputPressed(30);
		// }
	
	}
}

void AF4PlayerCharacter::Input_AbilityReleased(const FGameplayTag InputTag)
{
	if (UF4AbilitySystemComponent* F4ASC = Cast<UF4AbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		F4ASC->AbiliityInputReleased(InputTag);
		// if (InputTag == F4GameplayTags::Ability_Combat_Aim)
		// {
		// 	F4ASC->AbilityLocalInputReleased(30);
		// }
	}
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
