#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include"AbilitySystemComponent.h"
#include "F4AttributeSet.generated.h"


UCLASS()
class F4_API UF4AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UF4AttributeSet(); 
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override; 
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
#pragma region Health 
public:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Health")
	FGameplayAttributeData MaxHealth; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSet, MaxHealth);
	
#pragma endregion
	
#pragma region Stamina  
public:
	
	void HadnleStaminaRegen();
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSet, Stamina);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stamina")
	FGameplayAttributeData MaxStamina; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSet, MaxStamina);

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
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSet, WalkSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stats" )
	FGameplayAttributeData ATK; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSet, ATK);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stats" )
	FGameplayAttributeData DEF; 
	ATTRIBUTE_ACCESSORS_BASIC(UF4AttributeSet, DEF);

#pragma endregion
};
