// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Jump.generated.h"

/**
 * 
 */
UCLASS()
class F4_API UGA_Jump : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Jump(); 
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData
	) override;
};
