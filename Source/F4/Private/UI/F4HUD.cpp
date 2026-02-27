#include "UI/F4HUD.h"
#include "UI/StatBarWidget.h"
#include "UI/CrosshairWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "AbilitySystem/Attributes/F4AttributeSetWeapon.h"
#include "Components/TextBlock.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Firearm.h"
#include "Inventory/F4ItemInstance.h"
#include "System/F4GameplayTags.h"

void UF4HUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	Owner = GetOwningPlayerPawn();
	OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

	if (Owner)
	{
		EquipmentComp = Owner->FindComponentByClass<UF4EquipmentComponent>();
		InventoryComp = Owner->FindComponentByClass<UF4InventoryComponent>();
	}

	InitializeHealthBar();
	InitializeAmmoUI();
}

void UF4HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateCrosshair(InDeltaTime);
}

#pragma region Crosshair

void UF4HUD::UpdateCrosshair(float InDeltaTime)
{
	if (!Owner || !OwnerASC) return; 
	
	const bool bIsAiming = OwnerASC->HasMatchingGameplayTag(F4GameplayTags::State_Aiming);
	Crosshair->ToggleDot(bIsAiming); 
	
	const float BaseSpread = bIsAiming ? AimingSpread : NormalSpread;
	const float Speed = Owner->GetVelocity().Size2D();
	const float SpeedSpread = FMath::Pow(FMath::Clamp(Speed / 600.f, 0.f, 1.f), 1.5f) * 25.f; 
	
	RecoilSpread = FMath::FInterpTo(RecoilSpread, 0.f, InDeltaTime, 15.f);
	
	const float TargetSpread = BaseSpread + SpeedSpread + RecoilSpread;

	const float InterpSpeed = bIsAiming ? AimingInterpSpeed : NormalInterpSpeed;

	CurrentSpread = FMath::FInterpTo(CurrentSpread, TargetSpread, InDeltaTime, InterpSpeed);
	Crosshair->UpdateCrosshair(CurrentSpread);
}

void UF4HUD::AddRecoilImpulse(float ImpulseAmount)
{
	RecoilSpread += ImpulseAmount;
	RecoilSpread = FMath::Min(RecoilSpread, 500.f);
}

#pragma endregion 


#pragma region HealthBar

void UF4HUD::InitializeHealthBar()
{
	if (OwnerASC)
	{
		OwnerASC->GetGameplayAttributeValueChangeDelegate(UF4AttributeSetCharacter::GetHealthAttribute())
		   .AddUObject(this, &ThisClass::OnHealthChanged);
		
		float CurrentHealth = OwnerASC->GetNumericAttribute(UF4AttributeSetCharacter::GetHealthAttribute());
		float MaxHealth = OwnerASC->GetNumericAttribute(UF4AttributeSetCharacter::GetMaxHealthAttribute());
		UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void UF4HUD::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (!OwnerASC) return; 
	
	const float CurrentHealth = OwnerASC->GetNumericAttribute(UF4AttributeSetCharacter::GetHealthAttribute());
	const float MaxHealth = OwnerASC->GetNumericAttribute(UF4AttributeSetCharacter::GetMaxHealthAttribute());
	
	UpdateHealthBar(CurrentHealth, MaxHealth);
}

void UF4HUD::UpdateHealthBar(const float Current,const float Max) const
{
	HealthBar->UpdateStatBar(Current, Max);
}

#pragma endregion


#pragma region AmmoUI

void UF4HUD::InitializeAmmoUI()
{
	if (OwnerASC)
	{
		OwnerASC->GetGameplayAttributeValueChangeDelegate(
			UF4AttributeSetWeapon::GetCurrentAmmoAttribute()).AddUObject(this, &UF4HUD::OnCurrentAmmoChanged);

		float Current = OwnerASC->GetNumericAttribute(UF4AttributeSetWeapon::GetCurrentAmmoAttribute());
		FOnAttributeChangeData InitialData;
		InitialData.Attribute = UF4AttributeSetWeapon::GetCurrentAmmoAttribute();
		InitialData.NewValue = Current;
		InitialData.OldValue = Current;
		OnCurrentAmmoChanged(InitialData);
	}

	if (InventoryComp)
	{
		InventoryComp->OnInventoryUpdated.AddUniqueDynamic(this, &UF4HUD::HandleInventoryUpdate);
	}

	if (EquipmentComp)
	{
		EquipmentComp->OnActiveWeaponChanged.AddUniqueDynamic(this, &UF4HUD::HandleWeaponChange);
	}

	RefreshTotalAmmoUI();
}

void UF4HUD::OnCurrentAmmoChanged(const FOnAttributeChangeData& Data)
{
	if (CurrentAmmoText)
	{
		CurrentAmmoText->SetText(FText::AsNumber(FMath::FloorToInt(Data.NewValue)));
	}
}

void UF4HUD::HandleInventoryUpdate()
{
	RefreshTotalAmmoUI();
}

void UF4HUD::HandleWeaponChange(UF4ItemInstance* NewWeapon)
{
	RefreshTotalAmmoUI();
}

void UF4HUD::RefreshTotalAmmoUI()
{
	if (!TotalAmmoText || !InventoryComp || !EquipmentComp || !AmmoUIContainer)
	{
		return;
	}

	UF4ItemInstance* ActiveWeapon = EquipmentComp->GetActiveWeaponInstance();

	if (ActiveWeapon && ActiveWeapon->ItemDefinition)
	{
		const UF4ItemFragment_Firearm* FirearmFragment = ActiveWeapon->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Firearm>();
		if (FirearmFragment)
		{
			AmmoUIContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			int32 Total = InventoryComp->GetTotalItemCountByDefinition(FirearmFragment->AmmoItemDefinition);
			TotalAmmoText->SetText(FText::AsNumber(Total));
			return;
		}
	}

	AmmoUIContainer->SetVisibility(ESlateVisibility::Collapsed);
}

#pragma endregion
