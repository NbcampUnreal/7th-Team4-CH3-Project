#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "System/F4GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

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
		HandleStaminaRegen(); 
	}
}

void UF4AttributeSetCharacter::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// 최종 변환 정보를 바탕으로 사망처리
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetHealth() <= 0.0f)
		{
			FGameplayEventData Payload;
			Payload.EventTag = F4GameplayTags::Event_Character_Die;
			Payload.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
			Payload.Target = Data.Target.GetAvatarActor();
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				Data.Target.GetAvatarActor(), 
				F4GameplayTags::Event_Character_Die, 
				Payload
			);
		}
	}
}

void UF4AttributeSetCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UF4AttributeSetCharacter::InitializeStamina(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(F4GameplayTags::Character_State_NoRegenStamina).
		AddUObject(this, &ThisClass::OnNoRegenTagChanged);
	}
}

void UF4AttributeSetCharacter::OnNoRegenTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount == 0) HandleStaminaRegen(); 
}

void UF4AttributeSetCharacter::HandleStaminaRegen()
{
	// 1. Get Info from Owner or Avtar 
	AActor* Owner = GetOwningActor();
	if (Owner == nullptr) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner); 
	if (ASC == nullptr) return;
	
	// 2. Check Conditions 
	FGameplayTagContainer Container;
	Container.AddTag(F4GameplayTags::Character_State_NoRegenStamina); // 재생 불가 
	Container.AddTag(F4GameplayTags::Character_State_RegenStamina); // 재생 중
	if (ASC->HasAnyMatchingGameplayTags(Container)) return; 
	
	// 3. Check Stamina Value 
	float CurrentStaminaValue = GetStamina(); 
	float MaxStaminaValue = GetMaxStamina(); 
	
	if (CurrentStaminaValue < MaxStaminaValue)
	{
		// 4. Apply Effect 
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
		// 5. Remove Effect 
		ASC->RemoveActiveGameplayEffectBySourceEffect(StaminaRegenGEClass, ASC);
	}
}
