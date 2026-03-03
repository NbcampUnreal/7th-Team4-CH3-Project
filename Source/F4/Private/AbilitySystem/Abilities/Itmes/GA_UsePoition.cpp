#include "AbilitySystem/Abilities/Itmes/GA_UsePoition.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

void UGA_UsePoition::OnConsumeActivated(UF4ItemInstance* Item)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	
	float CalculatedDuration = Duration;
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle =
		ASC->MakeOutgoingSpec(ConsumableEffectClass, 1.0f, EffectContextHandle);

	if (SpecHandle.IsValid())
	{
		float GEDuration = SpecHandle.Data->GetDuration();

		if (GEDuration > 0.0f)
		{
			CalculatedDuration = GEDuration;
		}
	}

	ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, CalculatedDuration);
	WaitDelay->OnFinish.AddDynamic(this, &UGA_UsePoition::OnDurationEnded);
	WaitDelay->ReadyForActivation();
}

void UGA_UsePoition::OnDurationEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
