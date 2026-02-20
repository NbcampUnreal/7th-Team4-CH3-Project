// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInst/F4PistolAnimInst.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "System/F4GameplayTags.h"

void UF4PistolAnimInst::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	UpdateAimOffset(); 
	UpdateAiming(); 
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

void UF4PistolAnimInst::UpdateAiming()
{
	if (!OwnerCharacter) return;
	
	IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(OwnerCharacter);
	if (!Interface) return;
	
	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(Interface->GetAbilitySystemComponent());
	if (!ASC) return;
	
	bAiming = ASC->HasMatchingGameplayTag(F4GameplayTags::State_Aiming);
}
