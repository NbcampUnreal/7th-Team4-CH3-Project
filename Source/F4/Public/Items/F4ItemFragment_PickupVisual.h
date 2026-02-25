#pragma once

#include "CoreMinimal.h"
#include "Inventory/F4ItemFragment.h"
#include "F4ItemFragment_PickupVisual.generated.h"

UCLASS()
class F4_API UF4ItemFragment_PickupVisual : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Visual")
	TObjectPtr<UStaticMesh> PickupMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Visual")
	FVector PickupScale = FVector(1.0f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|SubVisual")
	TObjectPtr<UStaticMesh> SubPickupMesh;
	
	// TODO: 빌드가 된 후 에디터 실행해서 소캣 이름 확인하기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|SubVisual")
	FName SubMeshSocketName = TEXT("MagazineSocket");
};