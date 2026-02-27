#include "Characters/Base/F4CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetWeapon.h"


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
	
	ASC = CreateDefaultSubobject<UF4AbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 
	
	AttributeSet = CreateDefaultSubobject<UF4AttributeSetCharacter>(TEXT("AttributeSet"));
	WeaponAttributeSet = CreateDefaultSubobject<UF4AttributeSetWeapon>(TEXT("WeaponAttributeSet"));
}

void AF4CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilitySystem(); 
}

void AF4CharacterBase::InitAbilitySystem()
{
	if (!ASC) return; 
	
	ASC->InitAbilityActorInfo(this, this);
	
	ASC->GiveDefaultAbility();
	
	InitializeAttributes(); 
}

void AF4CharacterBase::InitializeAttributes()
{
	if (!ASC || !AttributeSet) return;
	
	const float CurrentSpeed = ASC->GetNumericAttribute(UF4AttributeSetCharacter::GetWalkSpeedAttribute());
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	
	ASC->GetGameplayAttributeValueChangeDelegate(UF4AttributeSetCharacter::GetWalkSpeedAttribute()).
		AddUObject(this, &ThisClass::OnSpeedChanged);
}

void AF4CharacterBase::OnSpeedChanged(const FOnAttributeChangeData& Data)
{
	float NewSpeed = ASC->GetNumericAttribute(UF4AttributeSetCharacter::GetWalkSpeedAttribute()); 
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

UAbilitySystemComponent* AF4CharacterBase::GetAbilitySystemComponent() const
{
	return ASC; 
}
