#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_PlaySound.generated.h"

UCLASS()
class F4_API UAN_PlaySound : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference
	) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	FName BoneName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundBase* SoundForPlay;
	
};
