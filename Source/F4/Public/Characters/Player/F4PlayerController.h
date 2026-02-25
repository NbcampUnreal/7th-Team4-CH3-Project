#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "F4PlayerController.generated.h"

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

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UF4InventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

private:
	UPROPERTY()
	TObjectPtr<UF4InventoryWidget> InventoryWidgetInstance;
};
