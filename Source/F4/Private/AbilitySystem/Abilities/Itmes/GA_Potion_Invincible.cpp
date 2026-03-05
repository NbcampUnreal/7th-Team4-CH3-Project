#include "AbilitySystem/Abilities/Itmes/GA_Potion_Invincible.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_UI.h"
#include "Inventory/F4ItemInstance.h"
#include "Inventory/F4BuffComponent.h"
#include "System/F4GameplayTags.h"

UGA_Potion_Invincible::UGA_Potion_Invincible()
{
	// 트리거 설정은 GA_ConsumableBase 생성자에서 처리
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Combat_Invisible));
}

void UGA_Potion_Invincible::OnConsumeActivated(UF4ItemInstance* Item)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	ACharacter* AvatarCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	
	UTexture2D* PotionIcon = nullptr;
	
	if (Item && Item->ItemDefinition)
	{
		if (const UF4ItemFragment_UI* UIFrag = Item->ItemDefinition->FindFragmentByClass<UF4ItemFragment_UI>())
		{
			PotionIcon = UIFrag->ItemIcon;
		}
	}
	
	if (AvatarCharacter && ASC)
	{
		if (ConsumableEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = 
				ASC->MakeOutgoingSpec(ConsumableEffectClass, 1.0f, EffectContext);
			
			ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			
			if (UF4BuffComponent* BuffComp = AvatarCharacter->FindComponentByClass<UF4BuffComponent>())
			{
				BuffComp->AddBuffToUI(ActiveEffectHandle, PotionIcon);
			}
		}
		if (USkeletalMeshComponent* Mesh = AvatarCharacter->GetMesh())
		{
			if (TransparentMaterial)
			{
				OriginalMaterials.Empty();
				
				for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
				{
					OriginalMaterials.Add(Mesh->GetMaterial(i));
					Mesh->SetMaterial(i, TransparentMaterial);
				}
			}
		}
		
		AddGameplayCue(ASC, AvatarCharacter);
	}

	UAbilityTask_WaitGameplayTagAdded* WaitFireTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this, FGameplayTag::RequestGameplayTag(TEXT("Ability.Combat.Fire")));
	WaitFireTag->Added.AddDynamic(this, &UGA_Potion_Invincible::OnActionDetected);
	WaitFireTag->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* WaitSprintTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this, FGameplayTag::RequestGameplayTag(TEXT("Ability.Movement.Sprint")));
	WaitSprintTag->Added.AddDynamic(this, &UGA_Potion_Invincible::OnActionDetected);
	WaitSprintTag->ReadyForActivation();

	UAbilityTask_WaitGameplayEffectRemoved* WaitGERemoved = 
		UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(this, ActiveEffectHandle);
	WaitGERemoved->OnRemoved.AddDynamic(this, &UGA_Potion_Invincible::OnDurationEnded);
	WaitGERemoved->ReadyForActivation();
}

void UGA_Potion_Invincible::OnConsumeEnded()
{
	if (ACharacter* AvatarCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		if (USkeletalMeshComponent* Mesh = AvatarCharacter->GetMesh())
		{
			for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
			{
				if (OriginalMaterials[i])
					Mesh->SetMaterial(i, OriginalMaterials[i]);
			}
		}
	}

	if (UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get())
	{
		if (ActiveEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
			ActiveEffectHandle.Invalidate();
		}
		
		ASC->RemoveGameplayCue(F4GameplayTags::GameplayCue_Item_Invincible);
	}
	
	OriginalMaterials.Empty();
}

void UGA_Potion_Invincible::AddGameplayCue(UAbilitySystemComponent* ASC, ACharacter* PlayerCharacter)
{
	if (!ASC || !PlayerCharacter)
	{
		return;
	}
	ASC->ExecuteGameplayCue(F4GameplayTags::GameplayCue_Item_Gulp);
	ASC->AddGameplayCue(F4GameplayTags::GameplayCue_Item_Invincible);
}

void UGA_Potion_Invincible::OnDurationEnded(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Potion_Invincible::OnActionDetected()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
