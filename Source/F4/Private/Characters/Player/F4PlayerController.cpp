#include "Characters/Player/F4PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory/F4InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DeathMenuWidget.h"
#include "UI/F4InventoryWidget.h"
#include "UI/PauseMenuWidget.h"

void AF4PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		if (MouseLookMappingContext)
		{
			Subsystem->AddMappingContext(MouseLookMappingContext, 1);
		}
	}
}

void AF4PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (InventoryAction)
		{
			EnhancedInputComponent->BindAction(
				InventoryAction,
				ETriggerEvent::Started,
				this,
				&ThisClass::ToggleInventory
			);
		}
		
		if (PauseAction)
		{
			EnhancedInputComponent->BindAction(
				PauseAction,
				ETriggerEvent::Started,
				this, 
				&ThisClass::TogglePauseMenu
			);
	
		}
	}
}

void AF4PlayerController::ToggleInventory()
{
	if (!InventoryWidgetInstance && InventoryWidgetClass)
	{
		InventoryWidgetInstance = CreateWidget<UF4InventoryWidget>(this, InventoryWidgetClass);
	}

	if (!InventoryWidgetInstance) return;

	if (!InventoryWidgetInstance->IsInViewport())
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			InventoryWidgetInstance->InventoryComponent = ControlledPawn->FindComponentByClass<UF4InventoryComponent>();
		}

		InventoryWidgetInstance->AddToViewport();
		InventoryWidgetInstance->RefreshInventory();

		SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidgetInstance->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	else
	{
		InventoryWidgetInstance->RemoveFromParent();

		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}

void AF4PlayerController::TogglePauseMenu()
{
	UE_LOG(LogTemp, Log, TEXT("TogglePauseMenu"));
	if (PauseWidgetInstance && PauseWidgetInstance->IsInViewport())
	{
		PauseWidgetInstance->OnContinueClicked(); // 위젯 내부의 닫기 로직 활용
		return;
	}
	
	if (!PauseWidgetInstance && PauseWidgetClass)
	{
		PauseWidgetInstance = CreateWidget<UPauseMenuWidget>(this, PauseWidgetClass);
	}
	
	if (PauseWidgetInstance)
	{
		PauseWidgetInstance->AddToViewport();
		
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(PauseWidgetInstance->TakeWidget());
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}

void AF4PlayerController::OpenDeathMenu()
{
	if (!DeathWidgetInstance && DeathWidgetClass)
	{
		DeathWidgetInstance = CreateWidget<UDeathMenuWidget>(this, DeathWidgetClass);
	}
	
	if (DeathWidgetInstance)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		DeathWidgetInstance->AddToViewport();
		
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(DeathWidgetInstance->TakeWidget());
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
	
}
