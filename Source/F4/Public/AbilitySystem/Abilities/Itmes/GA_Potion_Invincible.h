#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Itmes/GA_ConsumableBase.h"
#include "GA_Potion_Invincible.generated.h"

class AF4PlayerCharacter;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion|Visual")
	TObjectPtr<UMaterialInterface> TransparentMaterial;

protected:
	UFUNCTION()
	void OnDurationEnded(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo);

	UFUNCTION()
	void OnActionDetected();
	
	UFUNCTION()
	void AddGameplayCue(UAbilitySystemComponent* ASC, ACharacter* PlayerCharacter);

private:
	FActiveGameplayEffectHandle ActiveEffectHandle;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OriginalMaterials;
};
