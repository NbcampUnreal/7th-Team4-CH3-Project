#include "Items/Weapons/F4Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "System/F4GameplayTags.h"

AF4Projectile::AF4Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(CollisionRadius);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AF4Projectile::OnHit);

	// 캐릭터가 날아가는 총알을 밟고 올라타는 물리적 버그를 막기 위함
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	CollisionComponent->SetNotifyRigidBodyCollision(true);

	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = InitSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	InitialLifeSpan = LifeTimeSeconds;
}

void AF4Projectile::BeginPlay()
{
	Super::BeginPlay();

	if (GetInstigator() && CollisionComponent)
	{
		CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void AF4Projectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetInstigator() || OtherActor == this)
	{
		return;
	}

	if (GetInstigator())
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetInstigator();
		Payload.Target = OtherActor;

		FHitResult* MutableHit = const_cast<FHitResult*>(&Hit);
		Payload.TargetData.Add(new FGameplayAbilityTargetData_SingleTargetHit(*MutableHit));

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), F4GameplayTags::Event_Hit_Damage, Payload);
	}

	// TODO: particle or sound (Gameplay Cue)
	Destroy();
}
