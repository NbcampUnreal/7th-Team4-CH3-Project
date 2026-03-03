#include "Animation/Notifies/AN_PlaySound.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "System/F4GameInstance.h"

void UAN_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character || Character->GetCurrentMontage() != nullptr) return; 
	
	FVector PlayLocation = MeshComp->GetBoneLocation(BoneName);
	
	UGameplayStatics::PlaySoundAtLocation(Character, SoundForPlay, PlayLocation);
	
}
	
	
