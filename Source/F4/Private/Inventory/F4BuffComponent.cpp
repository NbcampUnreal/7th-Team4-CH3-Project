#include "Inventory/F4BuffComponent.h"


UF4BuffComponent::UF4BuffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UF4BuffComponent::AddBuffToUI(FActiveGameplayEffectHandle EffectHandle, UTexture2D* BuffIcon)
{
	if (OnBuffApplied.IsBound())
	{
		OnBuffApplied.Broadcast(EffectHandle, BuffIcon);
	}
}
