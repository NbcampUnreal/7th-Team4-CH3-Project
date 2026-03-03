

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FootStepSoundDataAsset.generated.h"

class UPhysicalMaterial;
class USoundBase;

UCLASS()
class F4_API UFootStepSoundDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	TMap<TObjectPtr<UPhysicalMaterial>, TObjectPtr<USoundBase>> FootstepMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	TObjectPtr<USoundBase> DefaultSound;
};
