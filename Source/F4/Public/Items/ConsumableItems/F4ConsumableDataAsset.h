#pragma once

#include "CoreMinimal.h"
#include "Items/F4ItemDataAsset.h"
#include "F4ConsumableDataAsset.generated.h"

class UGameplayEffect;

UCLASS()
class F4_API UF4ConsumableDataAsset : public UF4ItemDataAsset
{
	GENERATED_BODY()
	
public:
	UF4ConsumableDataAsset();
	
	// TODO: 어빌리티를 갖는 ConsumableItem은 ConsumableType에 따라 이펙트 또는 어빌리티를 갖게 수정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable|GAS")
	TSubclassOf<UGameplayEffect> ConsumeEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable|Stats")
	float EffectAmount = 30.0f;
};
