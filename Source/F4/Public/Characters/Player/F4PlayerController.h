#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "F4PlayerController.generated.h"

class UDeathMenuWidget;
class UPauseMenuWidget;
class UInputMappingContext;
class UF4InventoryWidget;
class UInputAction;

UCLASS()
class F4_API AF4PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ToggleInventory();
	
	void TogglePauseMenu();
public:	
	void OpenDeathMenu(); 

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UF4InventoryWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDeathMenuWidget> DeathWidgetClass; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MouseLookMappingContext;

	
private:
	UPROPERTY()
	TObjectPtr<UF4InventoryWidget> InventoryWidgetInstance;
	
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseWidgetInstance;
	
	UPROPERTY()
	TObjectPtr<UDeathMenuWidget> DeathWidgetInstance; 
};
