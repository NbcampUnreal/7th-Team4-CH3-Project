#include "Animation/F4AnimControlComponent.h"

#include "GameFramework/Character.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4ItemDefinition.h"
#include "Inventory/F4ItemFragment_Equipment.h"
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
			EquipmentComponent->OnActiveWeaponChanged.AddDynamic(this, &ThisClass::OnWeaponChanged);
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
		const UF4ItemFragment_Equipment* EquipmentFrag = NewItem->ItemDefinition->FindFragmentByClass<UF4ItemFragment_Equipment>();
		if (EquipmentFrag && EquipmentFrag->AnimLayerClass)
		{
			TargetLayer = EquipmentFrag->AnimLayerClass;
		}
	}

	AnimInstance->LinkAnimClassLayers(TargetLayer);
}
