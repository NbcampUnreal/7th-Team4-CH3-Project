// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/F4CharacterBase.h"
#include "F4PlayerCharacter.generated.h"
#include "Interface/Interactable.h"

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
	
public:
	// input Functions 
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	void Zoom(const FInputActionValue& Value);
	
	void Jump();
	
	void Roll(); 
	
	void Crouch(); 
	
	void StartSprint();
	void StopSprint();
	
	void Attack(); 
	
	void Interact();
	
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
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ZoomAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* RollAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
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
