#include "AbilitySystem/Abilities/Itmes/GA_UsePoition.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/Character.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4BuffComponent.h"

void UGA_UsePoition::OnConsumeActivated(UF4ItemInstance* Item)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	ACharacter* AvatarCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	
	float CalculatedDuration = Duration;
	UTexture2D* PotionIcon = nullptr;
	
	if (Item && Item->ItemDefinition)
	{
		if (const UF4ItemFragment_UI* UIFrag = Item->ItemDefinition->FindFragmentByClass<UF4ItemFragment_UI>())
		{
			PotionIcon = UIFrag->ItemIcon;
		}
	}
	
	if (AvatarCharacter && ASC)
	{
		if (ConsumableEffectClass)
		{
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
		}
		
		if (UF4BuffComponent* BuffComp = AvatarCharacter->FindComponentByClass<UF4BuffComponent>())
		{
			BuffComp->AddBuffToUI(CalculatedDuration, PotionIcon);
		}
	}
	
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, CalculatedDuration);
	WaitDelay->OnFinish.AddDynamic(this, &UGA_UsePoition::OnDurationEnded);
	WaitDelay->ReadyForActivation();
}

void UGA_UsePoition::OnDurationEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
