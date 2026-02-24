#include "Animation/AnimInst/F4PistolAnimInst.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "System/F4GameplayTags.h"

void UF4PistolAnimInst::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	UpdateAimOffset(); 
}

void UF4PistolAnimInst::UpdateAimOffset()
{
	if (!OwnerCharacter) return; 
	
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator ActorRotation = OwnerCharacter->GetActorRotation();
	
	FRotator Delta = (ControlRotation- ActorRotation).GetNormalized();
	
	AimPitch = Delta.Pitch;
	AimYaw = Delta.Yaw;
}
