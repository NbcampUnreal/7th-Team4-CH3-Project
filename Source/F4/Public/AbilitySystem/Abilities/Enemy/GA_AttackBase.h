#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_AttackBase.generated.h"

UCLASS()
class F4_API UGA_AttackBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_AttackBase();
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Attack | Montage")
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Attack | Montage")
	float MontageSpeed = 1.f;

	UFUNCTION()
	virtual void OnMontageCompleted();
	UFUNCTION()
	virtual void OnMontageCancelled();
};
