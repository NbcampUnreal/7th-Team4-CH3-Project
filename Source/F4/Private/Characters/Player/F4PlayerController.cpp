#include "Characters/Player/F4PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory/F4InventoryComponent.h"
#include "UI/F4InventoryWidget.h"

void AF4PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
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
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AF4PlayerController::ToggleInventory);
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
