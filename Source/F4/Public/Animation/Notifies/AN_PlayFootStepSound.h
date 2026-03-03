

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_PlayFootStepSound.generated.h"

class UFootStepSoundDataAsset; 

UCLASS()
class F4_API UAN_PlayFootStepSound : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference
	) override; 


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UFootStepSoundDataAsset> FootstepDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FName FootBoneName;
};
