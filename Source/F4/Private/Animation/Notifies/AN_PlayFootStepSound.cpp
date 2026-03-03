#include "Animation/Notifies/AN_PlayFootStepSound.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionQueryParams.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Animation/Data/FootStepSoundDataAsset.h"

void UAN_PlayFootStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	//  Check 
	if (!MeshComp || !MeshComp->GetOwner() || !FootstepDataAsset) return;
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character || Character->GetCurrentMontage() != nullptr) return;
	
	UWorld* World = Character->GetWorld();
	if (!World) return;
	
	// Trace 
	FHitResult HitResult;
	const FVector Start = MeshComp->GetBoneLocation(FootBoneName);
	const FVector End = Start + (FVector::DownVector * 50.f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	QueryParams.bReturnPhysicalMaterial = true;
	
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start, 
		End,
		ECC_Visibility,
		QueryParams
	);
	
	if (bHit)
	{
		UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();
		USoundBase* SoundToPlay = FootstepDataAsset->DefaultSound;
		
		if (PhysMat)
		{
			if (TObjectPtr<USoundBase>* FoundSound = FootstepDataAsset->FootstepMap.Find(PhysMat))
			{
				SoundToPlay = *FoundSound;
			}
		}
		
		if (SoundToPlay)
		{
			UGameplayStatics::PlaySoundAtLocation(World, SoundToPlay, HitResult.Location);
			UE_LOG(LogTemp, Warning, TEXT("PM : %s"), *PhysMat->GetName()); 
		}
	}
}

