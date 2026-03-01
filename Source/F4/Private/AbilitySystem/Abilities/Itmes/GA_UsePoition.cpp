#include "AbilitySystem/Abilities/Itmes/GA_UsePoition.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

void UGA_UsePoition::OnConsumeActivated(UF4ItemInstance* Item)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	ACharacter* AvatarCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddInstigator(AvatarCharacter, AvatarCharacter);
		
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ConsumableEffectClass, 1, ContextHandle);
		
	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
