// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInst/F4BaseAnimInst.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/F4GameplayTags.h"


void UF4BaseAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn) return;
	
	OwnerCharacter = Cast<ACharacter>(OwningPawn);	
	if (!OwnerCharacter) return;
	
	MovementComp = OwnerCharacter->GetCharacterMovement();

	AimingTag = F4GameplayTags::State_Aiming;
}

void UF4BaseAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!MovementComp) return;
	
	Velocity = MovementComp->Velocity;
	GroundSpeed = Velocity.Size2D(); 
	Direction = CalculateDirection(); 
	
	bIsFalling = MovementComp->IsFalling(); 
	bShouldMove = ShouldMove(); 
	
	IsAnyMontagePlaying();

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerCharacter);
	if (!ASC)
	{
		return;
	}

	bIsAiming = ASC->HasMatchingGameplayTag(AimingTag);
}

bool UF4BaseAnimInst::ShouldMove()
{
	return MovementComp->GetCurrentAcceleration() != FVector::ZeroVector && GroundSpeed > 0.01f; 
}

float UF4BaseAnimInst::CalculateDirection()
{
	if (Velocity.IsNearlyZero()) return 0.f; 
	
	FMatrix RotMatrix = FRotationMatrix(OwnerCharacter->GetActorRotation());
	FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
	FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
	FVector NormalizedVel = Velocity.GetSafeNormal2D();
		
	float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
	
	float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));
		
	float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
	if (RightCosAngle < 0)
	{
		ForwardDeltaDegree *= -1;
	}
		
	if (MovementComp->bOrientRotationToMovement == true)
	{
		ForwardDeltaDegree = FMath::Clamp(ForwardDeltaDegree, -45.f, 45.f);
	}
		
	return ForwardDeltaDegree;
}
