#include "Input/F4InputComponent.h"

UF4InputComponent::UF4InputComponent(const FObjectInitializer& Initializer)
{
}

void UF4InputComponent::AddInputMapping(const UF4InputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem)
{
	check(InputConfig);
	check(InputSubsystem);
}

void UF4InputComponent::RemoveInputMapping(const UF4InputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
}

void UF4InputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	
	BindHandles.Reset();
}
