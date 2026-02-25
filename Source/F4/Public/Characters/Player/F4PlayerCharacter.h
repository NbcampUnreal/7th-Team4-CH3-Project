#pragma once

#include "CoreMinimal.h"
#include "Animation/F4AnimControlComponent.h"
#include "Characters/Base/F4CharacterBase.h"
#include "Interface/Interactable.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "F4PlayerCharacter.generated.h"

struct FInputActionValue;

class UF4ItemDataAsset;
class UInputMappingContext;
class AF4WeaponActor;
class UCameraComponent;
class USpringArmComponent;
class UWidgetComponent;
class UF4HUD;
class UGaugeWidget;
class UInputAction;
class UF4InputConfig;

UCLASS()
class F4_API AF4PlayerCharacter : public AF4CharacterBase, public IInteractable
{
	GENERATED_BODY()

public:
	AF4PlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

public:
	// input Functions
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// input Functions
	void Input_Move(const FInputActionValue& Value);

	void Input_Look(const FInputActionValue& Value);

	void Input_Zoom(const FInputActionValue& Value);

	void Input_AbilityPressed(const FGameplayTag InputTag);
	void Input_AbilityReleased(const FGameplayTag InputTag);

public:
	// Interaction Functions 
	virtual void DoInteract(AActor* Interactor) override;

	virtual FText GetInteractionText() const override;

	UFUNCTION(BlueprintCallable)
	void ProcessItemPickup(const UF4ItemDataAsset* PickupItemData);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(const UF4WeaponDataAsset* NewWeaponData);

	void GrantWeaponAbilities(const UF4WeaponDataAsset* WeaponData);

protected:
	// UI Functions 
	void CreateHUD();

	void InitializeStaminaGauge();

	void OnStaminaChanged(const FOnAttributeChangeData& Data);

private:
	// Components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Inventory", meta = (AllowPrivateAccess = "true"))
	UF4InventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Equipment", meta = (AllowPrivateAccess = "true"))
	UF4EquipmentComponent* Equipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Animation",
		meta = (AllowPrivateAccess = "true"))
	UF4AnimControlComponent* AnimationControl;

protected:
	// Input Data
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UF4InputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultIMC;

public:
	// UI 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI | HUD")
	TSubclassOf<UUserWidget> HUDClass; // HUD Class

	UPROPERTY()
	TObjectPtr<UF4HUD> HUDWidget; // HUD

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI | HUD")
	TObjectPtr<UWidgetComponent> StaminaGaugeComponent;

	UPROPERTY()
	TObjectPtr<UGaugeWidget> StaminaGaugeWidget;

public:
	// Equipments 
	UPROPERTY()
	TObjectPtr<AF4WeaponActor> CurrentWeapon;

protected:
	// Zoom Variables 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera | Zoom")
	float MaxTargetArmLength = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera | Zoom")
	float MinTargetArmLength = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera | Zoom")
	float ZoomStep = 10.f;
};
