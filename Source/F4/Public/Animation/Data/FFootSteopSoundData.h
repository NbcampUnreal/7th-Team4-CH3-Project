#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundBase.h"
#include "FFootSteopSoundData.generated.h"


USTRUCT(BlueprintType)
struct FFootStepSoundData: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> Sound;
};
