#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "F4Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class F4_API AF4Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AF4Projectile();

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayEffectSpecHandle DamageSpecHandle;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Collision")
	float CollisionRadius = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Projectile")
	float InitSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Projectile")
	float MaxSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Self")
	float LifeTimeSeconds = 5.0f;

	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
};
