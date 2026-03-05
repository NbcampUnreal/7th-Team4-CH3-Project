#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "F4AbilitySystemComponent.generated.h"

class UF4AbilitySet;

UCLASS()
class F4_API UF4AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
	
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused); 
	
	void ClearAbilityInput();
	
public:
	void GiveDefaultAbility();
	
protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability | Default")
	UF4AbilitySet* DefaultAbilitySet;
};
