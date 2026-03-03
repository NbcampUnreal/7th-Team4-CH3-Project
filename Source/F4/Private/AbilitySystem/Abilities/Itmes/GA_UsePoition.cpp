#include "AbilitySystem/Abilities/Itmes/GA_UsePoition.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"
#include "GameFramework/Character.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4BuffComponent.h"

void UGA_UsePoition::OnConsumeActivated(UF4ItemInstance* Item)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	ACharacter* AvatarCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	
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

			
			ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			
			if (UF4BuffComponent* BuffComp = AvatarCharacter->FindComponentByClass<UF4BuffComponent>())
			{
				BuffComp->AddBuffToUI(ActiveEffectHandle, PotionIcon);
			}
			
			UAbilityTask_WaitGameplayEffectRemoved* WaitGERemoved = 
				UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(this, ActiveEffectHandle);
			WaitGERemoved->OnRemoved.AddDynamic(this, &UGA_UsePoition::OnDurationEnded);
			WaitGERemoved->ReadyForActivation();
		}
	}
}

void UGA_UsePoition::OnConsumeEnded()
{
	if (UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get())
	{
		if (ActiveEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
			ActiveEffectHandle.Invalidate();
		}
	}
}

void UGA_UsePoition::OnDurationEnded(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
