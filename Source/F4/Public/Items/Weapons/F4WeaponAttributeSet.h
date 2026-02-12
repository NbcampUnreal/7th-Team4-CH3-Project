#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "F4WeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class F4_API UF4WeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UF4WeaponAttributeSet();
	
	// 기본 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Stats")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UF4WeaponAttributeSet, BaseDamage)
	
	// 탄창 용량
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Stats")
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS(UF4WeaponAttributeSet, MaxAmmo)
	
	// 현재 탄약
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Stats")
	FGameplayAttributeData CurrentAmmo;
	ATTRIBUTE_ACCESSORS(UF4WeaponAttributeSet, CurrentAmmo)
	
	// Attribute가 변경되기 전에 호출되는 함수(Clamping)
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// Attribute가 변경된 후에 호출되는 함수
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override; 
};
