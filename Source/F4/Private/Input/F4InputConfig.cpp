#include "Input/F4InputConfig.h"

UF4InputConfig::UF4InputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UF4InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FF4InputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
			return Action.InputAction;
	}
	
	if (bLogNotFound)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
			*InputTag.ToString(), 
			*GetNameSafe(this)
		);
	}
	
	return nullptr;
}

const UInputAction* UF4InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FF4InputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
			return Action.InputAction;
	}
	
	if (bLogNotFound)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
			*InputTag.ToString(), 
			*GetNameSafe(this)
		);
	}
	
	return nullptr;
}
