// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FText GetInteractionText() const override;

protected:
	// input Functions
	void Input_Move(const FInputActionValue& Value);

	void Input_Look(const FInputActionValue& Value);

	void Input_Zoom(const FInputActionValue& Value);

	void Input_AbilityPressed(const FGameplayTag InputTag);
	void Input_AbilityReleased(const FGameplayTag InputTag);


public:
	UFUNCTION(BlueprintCallable)
	void ProcessItemPickup(const UF4ItemDataAsset* PickupItemData);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(const UF4WeaponDataAsset* NewWeaponData);

	void GrantWeaponAbilities(const UF4WeaponDataAsset* WeaponData);

	UPROPERTY()
	TObjectPtr<AF4WeaponActor> CurrentWeapon;

	virtual void DoInteract(AActor* Interactor) override;
	
protected:
	void CreateHUD(); 
	
	void InitializeStaminaGauge();
	
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	
#pragma region component

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta = (AllowPrivateAccess = "true"))
	UF4InventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment", meta = (AllowPrivateAccess = "true"))
	UF4EquipmentComponent* Equipment;

#pragma endregion

#pragma region Input Data

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UF4InputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultIMC;

#pragma endregion

#pragma region Camera

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera | Zoom")
	float MaxTargetArmLength = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera | Zoom")
	float MinTargetArmLength = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera | Zoom")
	float ZoomStep = 10.f;

#pragma endregion

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="UI | HUD")
	TSubclassOf<UUserWidget> HUDClass;
	
	UPROPERTY()
	TObjectPtr<UF4HUD> HUDWidget;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="UI | HUD")
	TObjectPtr<UWidgetComponent> StaminaGaugeComponent; 
	
	UPROPERTY()
	TObjectPtr<UGaugeWidget> StaminaGaugeWidget;
};
