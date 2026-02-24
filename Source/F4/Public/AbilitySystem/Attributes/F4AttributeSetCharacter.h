#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include"AbilitySystemComponent.h"
#include "F4AttributeSetCharacter.generated.h"


UCLASS()
class F4_API UF4AttributeSetCharacter : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UF4AttributeSetCharacter();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override; 
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue);
	
	void InitializeStamina(UAbilitySystemComponent* ASC); 
	
	UFUNCTION()
	void OnNoRegenTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UFUNCTION()
	void HandleStaminaRegen();
	
#pragma region Health 
public:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSetCharacter, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Health")
	FGameplayAttributeData MaxHealth; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSetCharacter, MaxHealth);
	
#pragma endregion
	
#pragma region Stamina  
public:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSetCharacter, Stamina);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stamina")
	FGameplayAttributeData MaxStamina; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSetCharacter, MaxStamina);

	// Stamina Regen Gameplay Effect 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute | Stamina | Regen")
	TSubclassOf<UGameplayEffect> StaminaRegenGEClass; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute | Stamina | Regen")
	FGameplayTag StaminaRegenTag;
#pragma endregion
	
#pragma region Speed  
public:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stats" )
	FGameplayAttributeData WalkSpeed; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSetCharacter, WalkSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stats" )
	FGameplayAttributeData ATK; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSetCharacter, ATK);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stats" )
	FGameplayAttributeData DEF; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSetCharacter, DEF);

#pragma endregion
};
