

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "F4CharacterBase.generated.h"

class UGameplayAbility; 
class UF4AttributeSet; 

struct FGameplayAbilitySpecHandle;
UCLASS()
class F4_API AF4CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AF4CharacterBase();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
#pragma region Component
	
	
#pragma endregion

	
#pragma region Gameplay Abilities 
protected:
	
	void OnSpeedAttributeChanged(); 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | Attributes")
	UF4AttributeSet* AttributeSet; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Abilities")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Abilities")
	TArray<FGameplayAbilitySpecHandle> InitailAbilitySpecHandles;
	
#pragma endregion 
	

};
