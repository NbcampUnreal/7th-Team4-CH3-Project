#include "Characters/Base/F4CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
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
	
	ASC = CreateDefaultSubobject<UF4AbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 
	
	AttributeSet = CreateDefaultSubobject<UF4AttributeSetCharacter>(TEXT("AttributeSet"));
}

void AF4CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 1. Init AbilitySystemComponent 
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		ASC->GiveDefaultAbility();
	}
	
	
	
	// 2. Give Abilities
	// for (const TSubclassOf<UGameplayAbility>& EachAbility : InitialAbilities)
	// {
	// 	if (EachAbility == nullptr) continue;
	// 	
	// 	FGameplayAbilitySpec EachSpec(EachAbility);
	//
	// 	// TODO: 매직넘버 제거 방안 고려 (인풋바인딩)
	// 	if (EachAbility->GetName().Contains(TEXT("Aim")))
	// 	{
	// 		EachSpec.InputID = 30;
	// 	}
	//
	// 	FGameplayAbilitySpecHandle EachSpecHandle = ASC->GiveAbility(EachSpec);
	// 	InitialAbilitySpecHandles.Add(EachSpecHandle);
	// }
	
	// 3. Bind Delegate 
	ASC->GetGameplayAttributeValueChangeDelegate(UF4AttributeSetCharacter::GetWalkSpeedAttribute()).
	AddUObject(this, &AF4CharacterBase::OnSpeedAttributeChanged);
	
	UpdateMoveSpeed(); // 처음에 초기화 호출 
}

void AF4CharacterBase::UpdateMoveSpeed()
{
	if (AttributeSet == nullptr && ASC != nullptr)
	{
		AttributeSet = const_cast<UF4AttributeSetCharacter*>(ASC->GetSet<UF4AttributeSetCharacter>());
	}
	
	float NewSpeed = AttributeSet->GetWalkSpeed();
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

UAbilitySystemComponent* AF4CharacterBase::GetAbilitySystemComponent() const
{
	return ASC; 
}


void AF4CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AF4CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AF4CharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AF4CharacterBase::OnSpeedAttributeChanged(const FOnAttributeChangeData& Data)
{
	UpdateMoveSpeed(); 
}
