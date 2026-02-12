// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Base/F4CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/F4AttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"


AF4CharacterBase::AF4CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 
	
	AttributeSet = CreateDefaultSubobject<UF4AttributeSet>(TEXT("AttributeSet"));
}

void AF4CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 1. Init AbilitySystemComponent 
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}
	
	// 2. Give Abilities
	for (const TSubclassOf<UGameplayAbility>& EachAbility : InitialAbilities)
	{
		if (EachAbility == nullptr) continue;
		
		FGameplayAbilitySpecHandle EachSpecHandle = ASC->GiveAbility(EachAbility);
		InitialAbilitySpecHandles.Add(EachSpecHandle);
	}
	
}

UAbilitySystemComponent* AF4CharacterBase::GetAbilitySystemComponent() const
{
	return ASC; 
}


void AF4CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Init speed 
	OnSpeedAttributeChanged(); 
}


void AF4CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AF4CharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AF4CharacterBase::OnSpeedAttributeChanged()
{
	if (AttributeSet == nullptr || GetCharacterMovement() == nullptr) return;
	
	float NewSpeed = AttributeSet->GetWalkSpeed();
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

