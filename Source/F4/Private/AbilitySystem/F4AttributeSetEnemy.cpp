#include "AbilitySystem/F4AttributeSetEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UF4AttributeSetEnemy::UF4AttributeSetEnemy()
{
	InitAttackRange(200.0f);
	InitTraceRange(1000.0f);
	InitMinTraceRange(150.0f);
	InitAttackCooldown(1.0f);
}

void UF4AttributeSetEnemy::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	// 사거리, 추적거리, 쿨타임 > 0
	if (Attribute == GetAttackRangeAttribute() || 
		Attribute == GetTraceRangeAttribute() || 
		Attribute == GetMinTraceRangeAttribute() || 
		Attribute == GetAttackCooldownAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UF4AttributeSetEnemy::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	AActor* TargetActor = GetOwningActor();
	if (APawn* Pawn = Cast<APawn>(TargetActor))
	{
		if (AAIController* AIC = Cast<AAIController>(Pawn->GetController()))
		{
			// 블랙보드값 갱신
			if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
			{
				if (Attribute == GetAttackRangeAttribute())
				{
					BB->SetValueAsFloat(TEXT("AttackRange"), NewValue);
				}
				
				if (Attribute == GetTraceRangeAttribute())
				{
					BB->SetValueAsFloat(TEXT("TraceRange"), NewValue);
				}
				
				if (Attribute == GetMinTraceRangeAttribute())
				{
					BB->SetValueAsFloat(TEXT("MinTraceRange"), NewValue);
				}
				
				if (Attribute == GetAttackCooldownAttribute())
				{
					BB->SetValueAsFloat(TEXT("AttackCooldown"), NewValue);
				}
			}
		}
	}
}
