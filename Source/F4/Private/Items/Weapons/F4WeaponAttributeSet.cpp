#include "Items/Weapons/F4WeaponAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UF4WeaponAttributeSet::UF4WeaponAttributeSet()
{
	InitBaseDamage(0.f);
	InitMaxAmmo(0.f);
	InitCurrentAmmo(0.f);
}

void UF4WeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	// CurrentAmmo를 0 ~ MaxAmmo로 Clamp
	if (Attribute == GetCurrentAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAmmo());
	}
}

void UF4WeaponAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// Effect 적용 후 처리
	if (Data.EvaluatedData.Attribute == GetCurrentAmmoAttribute())
	{
		// CurrentAmmo가 변경됐을 때
		SetCurrentAmmo(FMath::Clamp(GetCurrentAmmo(), 0.0f,GetMaxAmmo()));
		
		// TODO: CurrentAmmo가 0이면 발사 못하게 처리
	}
}
