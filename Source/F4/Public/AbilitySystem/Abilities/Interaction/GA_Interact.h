#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Interact.generated.h"

UCLASS()
class F4_API UGA_Interact : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Interact(); 
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interact | Trace")
	float InteractionDistance = 350.0f; 

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interact | Trace")
	float DrawTime = 1.f;  
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interact | Trace")
	bool bDrawTrace = true; 
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interact | Trace")
	FColor TraceColor = FColor::Yellow;
};
