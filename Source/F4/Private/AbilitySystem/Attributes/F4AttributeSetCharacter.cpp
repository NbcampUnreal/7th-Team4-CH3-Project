
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"

UF4AttributeSetCharacter::UF4AttributeSetCharacter()
{
	Health = 100.f;
	MaxHealth = 100.f; 
	
	Stamina = 100.f;
	MaxStamina = 100.f;
	
	ATK = 1.f;
	DEF = 1.f;
	WalkSpeed = 400.f; 
	
}

void UF4AttributeSetCharacter::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UF4AttributeSetCharacter::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetStaminaAttribute()) 
	{
		HadnleStaminaRegen(); 
	}
}

void UF4AttributeSetCharacter::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UF4AttributeSetCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UF4AttributeSetCharacter::HadnleStaminaRegen()
{
	
	// 1. Get Info from Owner or Avtar 
	AActor* Owner = GetOwningActor();
	if (Owner == nullptr) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner); 
	if (ASC == nullptr) return;
	
	if (ASC->HasMatchingGameplayTag(StaminaRegenTag)) return;
	
	
	// 2. Check Stamina Value 
	float CurrentStaminaValue = GetStamina(); 
	float MaxStaminaValue = GetMaxStamina(); 
	
	if (CurrentStaminaValue < MaxStaminaValue)
	{
		// 3. Apply Effect 
		if (StaminaRegenGEClass == nullptr) return;
		
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(Owner);
		
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
			StaminaRegenGEClass,
			1.f,
			EffectContext
		);
		
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	else if (CurrentStaminaValue >= MaxStaminaValue)
	{
		// 4. Remove Effect 
		ASC->RemoveActiveGameplayEffectBySourceEffect(StaminaRegenGEClass, ASC);
	}
}
