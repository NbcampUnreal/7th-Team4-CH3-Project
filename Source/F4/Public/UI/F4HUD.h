#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4HUD.generated.h"

struct FOnAttributeChangeData;

class UCrosshairWidget; 
class UStatBarWidget;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class F4_API UF4HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void UpdateCrosshair(float InDeltaTime);
	
	void AddRecoilImpulse(float ImpulseAmount);
	
	void InitializeHealthBar(); 
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	
	void UpdateHealthBar(const float Current, const float Max) const;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCrosshairWidget> Crosshair;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatBarWidget> HealthBar;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI | Crosshair")
	float AimingSpread = 15.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI | Crosshair")
	float NormalSpread = 60.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI | Crosshair")
	float AimingInterpSpeed = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI | Crosshair")
	float NormalInterpSpeed = 20.f;
	
private:
	UPROPERTY()
	TObjectPtr<APawn> Owner;

	UPROPERTY()
	UAbilitySystemComponent* OwnerASC;
	
	bool bAiming = false; 
	
	float CurrentSpread = 15.f;
	float RecoilSpread = 0.f;
};
