#include "Characters/Player/F4PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/ConsumableItems/F4ConsumableDataAsset.h"

#include "EnhancedInputSubsystems.h"
#include "Input/F4InputComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "System/F4GameplayTags.h"
#include "Items/Weapons/F4WeaponActor.h"
#include "Items/Weapons/F4WeaponDataAsset.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "UI/F4HUD.h"
#include "UI/GaugeWidget.h"



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

	StaminaGaugeComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StaminaGauge"));
	StaminaGaugeComponent->SetupAttachment(RootComponent);
	StaminaGaugeComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StaminaGaugeComponent->SetDrawSize(FVector2D(124.f, 124.f));
	
	Inventory = CreateDefaultSubobject<UF4InventoryComponent>(TEXT("Inventory"));

	Equipment = CreateDefaultSubobject<UF4EquipmentComponent>(TEXT("Equipment"));
}

void AF4PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CreateHUD(); 
	
	if (!StaminaGaugeWidget && StaminaGaugeComponent)
	{
		StaminaGaugeWidget = Cast<UGaugeWidget>(StaminaGaugeComponent->GetUserWidgetObject());
	}
	
	InitializeStaminaGauge(); 
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
	}
}

void AF4PlayerCharacter::Input_AbilityReleased(const FGameplayTag InputTag)
{
	if (UF4AbilitySystemComponent* F4ASC = Cast<UF4AbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		F4ASC->AbiliityInputReleased(InputTag);
	}
}

void AF4PlayerCharacter::ProcessItemPickup(const UF4ItemDataAsset* PickupItemData)
{
	if (!PickupItemData) return;

	switch (PickupItemData->ItemType)
	{
	case EF4ItemType::Weapon:
		{
			const UF4WeaponDataAsset* WeaponData = Cast<UF4WeaponDataAsset>(PickupItemData);
			EquipWeapon(WeaponData);
			break;
		}
	case EF4ItemType::Consumable:
		{
			const UF4ConsumableDataAsset* ConsumableData = Cast<UF4ConsumableDataAsset>(PickupItemData);
			// TODO: ConsumeItem(ConsumableData);
			break;
		}
	}
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

#pragma region UI Functions 
void AF4PlayerCharacter::CreateHUD()
{
	if (!HUDClass) return; 
	
	HUDWidget = CreateWidget<UF4HUD>(GetWorld(), HUDClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport(); 
	}
}

void AF4PlayerCharacter::InitializeStaminaGauge()
{
	if (!ASC) return;
	
	ASC->GetGameplayAttributeValueChangeDelegate(UF4AttributeSetCharacter::GetStaminaAttribute())
		.AddUObject(this, &AF4PlayerCharacter::OnStaminaChanged);
	
	const float CurrentStamina = ASC->GetNumericAttribute(UF4AttributeSetCharacter::GetStaminaAttribute());
	const float MaxStaamina = ASC->GetNumericAttribute(UF4AttributeSetCharacter::GetMaxStaminaAttribute());
	
	StaminaGaugeWidget->UpdateGauge(CurrentStamina / MaxStaamina); 
}

void AF4PlayerCharacter::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (!StaminaGaugeWidget) return; 
	
	const float CurrentStamina = ASC->GetNumericAttribute(UF4AttributeSetCharacter::GetStaminaAttribute());
	const float MaxStaamina = ASC->GetNumericAttribute(UF4AttributeSetCharacter::GetMaxStaminaAttribute());
	
	StaminaGaugeWidget->UpdateGauge(CurrentStamina / MaxStaamina); 
}
#pragma endregion 