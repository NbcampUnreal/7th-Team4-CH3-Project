#include "Animation/F4AnimControlComponent.h"

#include "GameFramework/Character.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment.h"
#include "Inventory/F4ItemInstance.h"

UF4AnimControlComponent::UF4AnimControlComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultUnarmedLayer = nullptr;
	CachedMesh = nullptr;
}

void UF4AnimControlComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		CachedMesh = OwnerCharacter->GetMesh();

		if (UF4EquipmentComponent* EquipmentComponent = OwnerCharacter->FindComponentByClass<UF4EquipmentComponent>())
		{
			EquipmentComponent->OnActiveWeaponChanged.AddUniqueDynamic(this, &ThisClass::OnWeaponChanged);
			UpdateAnimationLayer(nullptr);
		}
	}
}

void UF4AnimControlComponent::OnWeaponChanged(UF4ItemInstance* NewItem)
{
	UpdateAnimationLayer(NewItem);
}

void UF4AnimControlComponent::UpdateAnimationLayer(UF4ItemInstance* NewItem)
{
	if (!CachedMesh)
	{
		return;
	}

	UAnimInstance* AnimInstance = CachedMesh->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	TSubclassOf<UAnimInstance> TargetLayer = DefaultUnarmedLayer;

	if (NewItem && NewItem->ItemDefinition)
	{
		for (const UF4ItemFragment* Fragment : NewItem->ItemDefinition->Fragments)
		{
			if (TSubclassOf<UAnimInstance> Layer = Fragment->GetEquipAnimLayer())
			{
				TargetLayer = Layer;
				break;
			}
		}
	}

	AnimInstance->LinkAnimClassLayers(TargetLayer);
}
