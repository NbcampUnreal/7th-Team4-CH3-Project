#include "Animation/Notifies/ANS_EnemySkillEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

UANS_EnemySkillEffect::UANS_EnemySkillEffect()
{
}

void UANS_EnemySkillEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !Template) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;
	
	FVector SpawnLocation = Owner->GetActorLocation();
	FRotator SpawnRotation = bUseCustomRotation ? CustomRotation : Owner->GetActorRotation();
	
	FHitResult HitResult;
	FVector TraceStart = SpawnLocation + FVector(0.f, 0.f, 150.f);
	FVector TraceEnd = SpawnLocation - FVector(0.f, 0.f, 500.f);
    
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	
	if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic, Params))
	{
		SpawnLocation = HitResult.Location;
	}
	
	UNiagaraComponent* NewComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		Template,
		SpawnLocation + LocationOffset,
		SpawnRotation
	);

	if (NewComp)
	{
		ActiveComponents.Add(MeshComp, NewComp);
	}
}

void UANS_EnemySkillEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (ActiveComponents.Contains(MeshComp))
	{
		if (UNiagaraComponent* Comp = ActiveComponents[MeshComp])
		{
			Comp->Deactivate();
		}
		ActiveComponents.Remove(MeshComp);
	}
}
