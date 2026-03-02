#include "Inventory/F4ItemFragment_Firearm.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/F4AttributeSetWeapon.h"
#include "Inventory/F4ItemInstance.h"
#include "System/F4GameplayTags.h"

void UF4ItemFragment_Firearm::OnItemEquipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance)
{
	if (!ASC || !Instance)
	{
		return;
	}

	if (EquipStatEffect)
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddInstigator(ASC->GetOwnerActor(), ASC->GetOwnerActor());
		ContextHandle.AddSourceObject(Instance);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EquipStatEffect, 1.0f, ContextHandle);
		if (SpecHandle.IsValid())
		{
			Instance->SetActiveGEHandle(ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()));
		}
	}

	float SavedAmmo = Instance->GetStatValue(F4GameplayTags::Weapon_Ammo_Loaded);
	ASC->SetNumericAttributeBase(UF4AttributeSetWeapon::GetCurrentAmmoAttribute(), SavedAmmo);
}

void UF4ItemFragment_Firearm::OnItemUnequipped(UAbilitySystemComponent* ASC, UF4ItemInstance* Instance)
{
	if (!ASC || !Instance)
	{
		return;
	}

	if (ASC->HasAttributeSetForAttribute(UF4AttributeSetWeapon::GetCurrentAmmoAttribute()))
	{
		float LeftAmmo = ASC->GetNumericAttribute(UF4AttributeSetWeapon::GetCurrentAmmoAttribute());
		Instance->DynamicStats.Add(F4GameplayTags::Weapon_Ammo_Loaded, LeftAmmo);
	}

	if (Instance->GetActiveGEHandle().IsValid())
	{
		ASC->RemoveActiveGameplayEffect(Instance->GetActiveGEHandle());
		Instance->InvalidateActiveGEHandle();
	}
}
