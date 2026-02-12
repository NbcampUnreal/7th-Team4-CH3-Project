#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "F4EnemyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class F4_API UF4EnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UF4EnemyAttributeSet();
	
	// 값이 바뀌기 전 수치 보정 (예: 쿨타임은 0보다 작을 수 없음)
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	// 공격 사거리
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Combat")
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UF4EnemyAttributeSet, AttackRange)

	// 추적 사거리
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Combat")
	FGameplayAttributeData TraceRange;
	ATTRIBUTE_ACCESSORS(UF4EnemyAttributeSet, TraceRange)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Combat")
	FGameplayAttributeData MinTraceRange;
	ATTRIBUTE_ACCESSORS(UF4EnemyAttributeSet, MinTraceRange)

	// 공격 쿨타임 (초 단위)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Combat")
	FGameplayAttributeData AttackCooldown;
	ATTRIBUTE_ACCESSORS(UF4EnemyAttributeSet, AttackCooldown)
	
};