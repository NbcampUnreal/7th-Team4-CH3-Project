#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "F4Projectile.generated.h"

class UParticleSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class F4_API AF4Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AF4Projectile();

	UPROPERTY(BlueprintReadWrite, Category = "Projectile")
	float DamagePayload = 0.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Collision")
	float CollisionRadius = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float InitSpeed = 8000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 8000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeTimeSeconds = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> BloodImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UParticleSystem> EnvironmentImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Headshot")
	float HeadshotMultiplier = 2.0f;

	// 캡슐 상단 몇 %를 헤드샷 판정 구역으로 볼지 (0.2 = 상위 20%)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Headshot")
	float HeadshotZoneRatio = 0.2f;

	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
};
