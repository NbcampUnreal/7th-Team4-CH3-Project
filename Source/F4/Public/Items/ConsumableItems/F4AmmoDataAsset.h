#pragma once

#include "CoreMinimal.h"
#include "Items/F4ItemDataAsset.h"
#include "F4AmmoDataAsset.generated.h"

UCLASS()
class F4_API UF4AmmoDataAsset : public UF4ItemDataAsset
{
	GENERATED_BODY()
	
public:
	UF4AmmoDataAsset();

	// 아이템을 주웠을 때 몇 발이 들어 올지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int32 AmmoAmount = 30;
};
