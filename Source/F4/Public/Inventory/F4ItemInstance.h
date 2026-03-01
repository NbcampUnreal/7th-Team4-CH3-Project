#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"
#include "UObject/NoExportTypes.h"
#include "F4ItemInstance.generated.h"

class UGameplayAbility;
class UF4ItemDefinition;

UCLASS(BlueprintType)
class F4_API UF4ItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UF4ItemInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UF4ItemDefinition> ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Stats")
	TMap<FGameplayTag, float> DynamicStats;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	UPROPERTY()
	FActiveGameplayEffectHandle ActiveGEHandle;

	UPROPERTY()
	FGameplayAbilitySpecHandle QuickSlotAbilityHandle;

	UFUNCTION(BlueprintCallable, Category = "Item|Stats")
	void AddStatValue(FGameplayTag StatTag, float Value);

	UFUNCTION(BlueprintPure, Category = "Item|Stats")
	float GetStatValue(FGameplayTag StatTag) const;
};
