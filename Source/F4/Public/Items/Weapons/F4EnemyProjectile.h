#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/F4Projectile.h"
#include "F4EnemyProjectile.generated.h"

class UNiagaraSystem;

UCLASS()
class F4_API AF4EnemyProjectile : public AF4Projectile
{
	GENERATED_BODY()
	
public:
	AF4EnemyProjectile();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnHit(
		UPrimitiveComponent* HitComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit
		) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> FlightEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> HitEffect;
};
