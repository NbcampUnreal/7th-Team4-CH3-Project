#include "AbilitySystem/Attributes/F4AttributeSetEnemy.h"

void UF4AttributeSetEnemy::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(
			GetHealthAttribute(), 
			GetMaxHealthAttribute(),
			NewValue, 
			GetHealthAttribute()
			);
	}
}

void UF4AttributeSetEnemy::AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute,
	const FGameplayAttribute& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetNumericValue(this);

	// 실제 값이 변했고, ASC가 유효할 때만 실행
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetNumericValue(this);
        
		// 비율 계산: (현재값 / 이전Max) * 새로운Max
		float NewValue = (CurrentMaxValue > 0.f) 
			? (CurrentValue * NewMaxValue / CurrentMaxValue) 
			: NewMaxValue;

		// 현재 체력/스태미나 값을 직접 업데이트 (Clamp는 나중에 수행됨)
		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Override, NewValue);
	}
}
