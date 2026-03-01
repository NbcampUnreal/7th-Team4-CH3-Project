#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Itmes/GA_ConsumableBase.h"
#include "GA_Potion_Invincible.generated.h"

class UMaterialInterface;
class UGameplayEffect;

UCLASS()
class F4_API UGA_Potion_Invincible : public UGA_ConsumableBase
{
	GENERATED_BODY()

public:
	UGA_Potion_Invincible();

protected:
	virtual void OnConsumeActivated(UF4ItemInstance* Item) override;
	virtual void OnConsumeEnded() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion|Effect")
	TSubclassOf<UGameplayEffect> InvincibilityEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion|Visual")
	TObjectPtr<UMaterialInterface> TransparentMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion|Time")
	float Duration = 5.0f;

protected:
	UFUNCTION()
	void OnDurationEnded();

	UFUNCTION()
	void OnActionDetected();

private:
	FActiveGameplayEffectHandle ActiveEffectHandle;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OriginalMaterials;
};
