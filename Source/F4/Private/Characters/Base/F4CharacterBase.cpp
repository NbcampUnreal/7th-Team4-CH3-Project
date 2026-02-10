// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Base/F4CharacterBase.h"
#include "AbilitySystem/F4AttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"


AF4CharacterBase::AF4CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* AF4CharacterBase::GetAbilitySystemComponent() const
{
	return ASC; 
}


void AF4CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Init Default speed 
	OnSpeedAttributeChanged(); 
}


void AF4CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AF4CharacterBase::OnSpeedAttributeChanged()
{
	if (AttributeSet == nullptr || GetCharacterMovement() == nullptr) return;
	
	float NewSpeed = AttributeSet->GetWalkSpeed();
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

