#include "Items/Weapons/F4EnemyProjectile.h"
#include "NiagaraFunctionLibrary.h"

AF4EnemyProjectile::AF4EnemyProjectile()
{
	InitialLifeSpan = 5.0f;
}
void AF4EnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
 
	if (FlightEffect && CollisionComponent)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			FlightEffect,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
}

void AF4EnemyProjectile::OnHit(
	UPrimitiveComponent* HitComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
	)
{
	if (!IsValid(OtherActor) || OtherActor == this) return;
	
	if (OtherActor->ActorHasTag(FName("Enemy")) || OtherActor == GetInstigator())
	{
		return; 
	}
	
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		if (HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
		
		Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
		
		Destroy();
		return;
	}
	
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
	
	Destroy();
}
