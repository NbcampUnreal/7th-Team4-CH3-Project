#include "Items/F4ItemFragment_PickupVisual.h"

#include "Components/StaticMeshComponent.h"

void UF4ItemFragment_PickupVisual::OnPickupActorSpawned(UStaticMeshComponent* MeshComp, UStaticMeshComponent* SubMeshComp)
{
	if (!MeshComp || !SubMeshComp)
	{
		return;
	}

	if (PickupMesh)
	{
		MeshComp->SetStaticMesh(PickupMesh);
		MeshComp->SetRelativeScale3D(PickupScale);
	}

	if (SubPickupMesh)
	{
		SubMeshComp->SetStaticMesh(SubPickupMesh);
		SubMeshComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SubMeshSocketName);
	}
	else
	{
		SubMeshComp->SetStaticMesh(nullptr);
	}
}
