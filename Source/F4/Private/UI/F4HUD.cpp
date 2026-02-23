// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/F4HUD.h"
#include "UI/StatBarWidget.h"
#include "UI/CrosshairWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetCharacter.h"
#include "System/F4GameplayTags.h"

void UF4HUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	Owner = GetOwningPlayerPawn();
	OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

	InitializeHealthBar(); 
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
	
	bool bIsAiming = OwnerASC->HasMatchingGameplayTag(F4GameplayTags::State_Aiming);
	Crosshair->ToggleDot(bIsAiming); 
	
	float BaseSpread = bIsAiming ? 15.f : 60.f;
	float Speed = Owner->GetVelocity().Size2D();
	float SpeedSpread = FMath::Pow(FMath::Clamp(Speed / 600.f, 0.f, 1.f), 1.5f) * 25.f; 
	
	RecoilSpread = FMath::FInterpTo(RecoilSpread, 0.f, InDeltaTime, 15.f);
	
	float TargetSpread = BaseSpread + SpeedSpread + RecoilSpread;
	float InterpSpeed = bIsAiming ? 20.f : 10.f;
	
	CurrentSpread = FMath::FInterpTo(CurrentSpread, TargetSpread, InDeltaTime, InterpSpeed);
	Crosshair->UpdateCrosshair(CurrentSpread);
}

void UF4HUD::AddRecoilImpulse(float ImpulseAmount)
{
	RecoilSpread += ImpulseAmount;
	RecoilSpread = FMath::Min(RecoilSpread, 50.f);
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