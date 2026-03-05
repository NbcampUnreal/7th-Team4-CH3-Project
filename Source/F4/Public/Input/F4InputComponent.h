#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/F4InputConfig.h"
#include "F4InputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;

UCLASS()
class F4_API UF4InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UF4InputComponent(const FObjectInitializer& Initializer);
	
	void AddInputMapping(const UF4InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
	
	void RemoveInputMapping(
		const UF4InputConfig* InputConfig,	
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem
	) const;
	
	void RemoveBinds(TArray<uint32>& BindHandles); 
	
public:
	template<class UserClass, typename FuncType>
	void BindNativeAction(
		const UF4InputConfig* InputConfig,
		const FGameplayTag& InputTag,
		ETriggerEvent TriggerEvent, 
		UserClass* Object, 
		FuncType Func, 
		bool bLogIfNotFound
	);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(
		const UF4InputConfig* InputConfig,
		UserClass* Object, 
		PressedFuncType PressedFunc, 
		ReleasedFuncType ReleasedFunc, 
		TArray<uint32>& BindHandles
	);
};

template <class UserClass, typename FuncType>
void UF4InputComponent::BindNativeAction(const UF4InputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA,TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UF4InputComponent::BindAbilityActions(const UF4InputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	
	for (const FF4InputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}
			
			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
