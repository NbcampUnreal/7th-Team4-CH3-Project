// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/F4PlayerCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"	
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DummyClass.h"



AF4PlayerCharacter::AF4PlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); 
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 110.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
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
		
		if (SprintAction)
		{
			EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AF4PlayerCharacter::StartSprint);
			EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &AF4PlayerCharacter::StopSprint);
		}
		
		if (CrouchAction)
		{
			// EIC->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Crouch);
		}
	
		if (AttackAction)
		{
			EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AF4PlayerCharacter::Attack);
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

void AF4PlayerCharacter::StartSprint()
{
	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Ability_Movement_Sprint);
	
	ASC->TryActivateAbilitiesByTag(Container);
}

void AF4PlayerCharacter::StopSprint()
{
	
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

void AF4PlayerCharacter::DoInteract(AActor* Interactor)
{
	
}
