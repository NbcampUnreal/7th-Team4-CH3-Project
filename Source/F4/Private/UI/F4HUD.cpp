// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/F4HUD.h"
#include "UI/StatBarWidget.h"
#include "UI/CrosshairWidget.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"

void UF4HUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	Owner = GetOwningPlayerPawn();
	if (Owner)
	{
		if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Owner))
		{
			OwnerASC = ASCInterface->GetAbilitySystemComponent();
		}
	}
}

void UF4HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateCrosshair(InDeltaTime);
}

void UF4HUD::UpdateCrosshair(float InDeltaTime)
{
	if (!Owner || !OwnerASC) return; 
    
	// 1. 조준 상태 확인 및 점(Dot) 표시 여부 결정
	bool bIsAiming = OwnerASC->HasMatchingGameplayTag(F4GameplayTags::State_Aiming);
	Crosshair->ToggleDot(bIsAiming); 
	
	float BaseSpread = bIsAiming ? 15.f : 60.f;
	float Speed = Owner->GetVelocity().Size2D();
	float SpeedSpread = FMath::Pow(FMath::Clamp(Speed / 600.f, 0.f, 1.f), 1.5f) * 25.f; 
	
	RecoilSpread = FMath::FInterpTo(RecoilSpread, 0.f, InDeltaTime, 15.f);
	
	// 4. 목표값 및 보간 속도 설정
	float TargetSpread = BaseSpread + SpeedSpread + RecoilSpread;
	float InterpSpeed = bIsAiming ? 20.f : 10.f;
    
	// 5. 실시간 보간 및 위젯 업데이트
	CurrentSpread = FMath::FInterpTo(CurrentSpread, TargetSpread, InDeltaTime, InterpSpeed);
	Crosshair->UpdateCrosshair(CurrentSpread);
}

void UF4HUD::AddRecoilImpulse(float ImpulseAmount)
{
	RecoilSpread += ImpulseAmount;
	RecoilSpread = FMath::Min(RecoilSpread, 50.f);
}
