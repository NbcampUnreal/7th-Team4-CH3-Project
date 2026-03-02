#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "F4HUD.generated.h"

class UF4ItemInstance;
class UF4InventoryComponent;
class UF4EquipmentComponent;
class UTextBlock;
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
	
	void InitializeTimeText();
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	
	void UpdateHealthBar(const float Current, const float Max) const;

	void InitializeAmmoUI();

	void OnCurrentAmmoChanged(const FOnAttributeChangeData& Data);

	UFUNCTION()
	void HandleInventoryUpdate();

	UFUNCTION()
	void HandleWeaponChange(UF4ItemInstance* NewWeapon);

	UFUNCTION()
	void UpdateTimeDisplay(int32 Hour, int32 Minute);
	
	void RefreshTotalAmmoUI();
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCrosshairWidget> Crosshair;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatBarWidget> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidget> AmmoUIContainer;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CurrentAmmoText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TotalAmmoText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Time;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TimeText; 
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
	TObjectPtr<UAbilitySystemComponent> OwnerASC;

	UPROPERTY()
	TObjectPtr<UF4EquipmentComponent> EquipmentComp;

	UPROPERTY()
	TObjectPtr<UF4InventoryComponent> InventoryComp;
	
	bool bAiming = false; 
	
	float CurrentSpread = 15.f;
	float RecoilSpread = 0.f;
	
protected:
	float DawnTime = -1.f;
	float DuskTime = -1.f;
	                                                                      
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	FString DayTimeText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	FString NightTimeText;
	
};
