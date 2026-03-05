#include "Animation/Notifies/AN_SendEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/Base/F4CharacterBase.h"

UAN_SendEvent::UAN_SendEvent()
{
}

void UAN_SendEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Actor = MeshComp->GetOwner();
	if (Actor == nullptr) return;

	AF4CharacterBase* Character = Cast<AF4CharacterBase>(Actor);
	if (Character == nullptr) return;

	FGameplayEventData EventData;
	EventData.Instigator = Character;
	EventData.Target = Character;


	for (FGameplayTag EventTag : EventTags)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, EventData);
	}
}
