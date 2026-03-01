#include "AbilitySystem/Abilities/Itmes/GA_Potion_Invincible.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Inventory/F4ItemInstance.h"
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

	if (AvatarCharacter && ASC)
	{
		if (ConsumableEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ConsumableEffectClass, 1.0f, EffectContext);
			ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

		if (USkeletalMeshComponent* Mesh = AvatarCharacter->GetMesh())
		{
			for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
			{
				OriginalMaterials.Add(Mesh->GetMaterial(i));
				if (TransparentMaterial)
				{
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("투명화"));
					Mesh->SetMaterial(i, TransparentMaterial);
				}
			}
		}
	}

	UAbilityTask_WaitGameplayTagAdded* WaitFireTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this, FGameplayTag::RequestGameplayTag(TEXT("Ability.Combat.Fire")));
	WaitFireTag->Added.AddDynamic(this, &UGA_Potion_Invincible::OnActionDetected);
	WaitFireTag->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* WaitSprintTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this, FGameplayTag::RequestGameplayTag(TEXT("Ability.Movement.Sprint")));
	WaitSprintTag->Added.AddDynamic(this, &UGA_Potion_Invincible::OnActionDetected);
	WaitSprintTag->ReadyForActivation();

	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, Duration);
	WaitDelay->OnFinish.AddDynamic(this, &UGA_Potion_Invincible::OnDurationEnded);
	WaitDelay->ReadyForActivation();
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
	}

	OriginalMaterials.Empty();
}

void UGA_Potion_Invincible::OnDurationEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Potion_Invincible::OnActionDetected()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
