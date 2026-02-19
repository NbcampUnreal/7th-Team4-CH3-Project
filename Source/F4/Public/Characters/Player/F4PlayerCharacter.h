// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/F4CharacterBase.h"
#include "Interface/Interactable.h"
#include "F4PlayerCharacter.generated.h"


class AF4WeaponActor;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;


class UInputAction;

UCLASS()
class F4_API AF4PlayerCharacter : public AF4CharacterBase, public IInteractable
{
	GENERATED_BODY()

public:
	AF4PlayerCharacter();

	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FText GetInteractionText() const override;

public:
	// input Functions
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Zoom(const FInputActionValue& Value);

	void Jump();

	void Roll();

	//	void Crouch();

	void ToggleSprint();

	void Attack();

	void Interact();

	void OnFire();

	void OnAimStarted();
	void OnAimReleased();

	void OnReload();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(const UF4WeaponDataAsset* NewWeaponData);

	void GrantWeaponAbilities(const UF4WeaponDataAsset* WeaponData);

	UPROPERTY()
	TObjectPtr<AF4WeaponActor> CurrentWeapon;

	virtual void DoInteract(AActor* Interactor) override;
#pragma region component

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components | Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion

#pragma region InputActions

protected:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ReloadAction;
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

#pragma region Tags
	UPROPERTY(EditAnywhere, Category="Tag")
	FGameplayTag Jumps;

#pragma endregion
};
