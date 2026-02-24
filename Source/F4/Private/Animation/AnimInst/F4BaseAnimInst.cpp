#include "Animation/AnimInst/F4BaseAnimInst.h"
#include "KismetAnimationLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Characters/Player/F4PlayerCharacter.h"
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

	if (AF4PlayerCharacter* F4Character = Cast<AF4PlayerCharacter>(OwnerCharacter))
	{
		bIsEquipped = F4Character->CurrentWeapon != nullptr;
	}
}

bool UF4BaseAnimInst::ShouldMove()
{
	return MovementComp->GetCurrentAcceleration() != FVector::ZeroVector && GroundSpeed > 0.01f; 
}

float UF4BaseAnimInst::CalculateDirection()
{
	if (Velocity.IsNearlyZero()) return 0.f; 
	
	FRotator Rotation = OwnerCharacter->GetController()->GetControlRotation();
	
	return UKismetAnimationLibrary::CalculateDirection(Velocity,Rotation); 
}
