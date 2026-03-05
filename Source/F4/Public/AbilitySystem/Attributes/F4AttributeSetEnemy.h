#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "F4AttributeSetEnemy.generated.h"

UCLASS()
class F4_API UF4AttributeSetEnemy : public UF4AttributeSetCharacter
{
	GENERATED_BODY()
protected:
	// 적 전용: MaxHealth 변경 시 Health 비율 조정 로직 추가
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// 비율 조정 헬퍼 함수
	void AdjustAttributeForMaxChange(
		const FGameplayAttribute& AffectedAttribute, 
		const FGameplayAttribute& MaxAttribute, 
		float NewMaxValue, 
		const FGameplayAttribute& AffectedAttributeProperty
		);
};
