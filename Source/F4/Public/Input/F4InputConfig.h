#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "F4InputConfig.generated.h"

class UInputAction; 

USTRUCT(BlueprintType)
struct FF4InputAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Category = "InputTag"))
	FGameplayTag InputTag; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Category = "InputTag"))
	ETriggerEvent TriggerEvent = ETriggerEvent::Triggered; 
};

UCLASS()
class F4_API UF4InputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UF4InputConfig(const FObjectInitializer& ObjectInitializer); 
	
	UFUNCTION(BlueprintCallable)
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UFUNCTION(BlueprintCallable)
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FF4InputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FF4InputAction> AbilityInputActions;
	
};
