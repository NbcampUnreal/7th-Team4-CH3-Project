#include "AbilitySystem/Abilities/Enemy/GA_EnemyDeath.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/F4GameplayTags.h"
#include "Items/F4DropItem.h"
#include "Kismet/GameplayStatics.h"

UGA_EnemyDeath::UGA_EnemyDeath()
{
	SetAssetTags(FGameplayTagContainer(F4GameplayTags::Ability_Death));

	ActivationOwnedTags.AddTag(F4GameplayTags::State_Dead);
	
	CancelAbilitiesWithTag.AddTag(F4GameplayTags::Ability_Attack);
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = F4GameplayTags::Event_Character_Die;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    
	AbilityTriggers.Add(TriggerData);
}

void UGA_EnemyDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	AF4EnemyBase* Enemy = Cast<AF4EnemyBase>(Avatar);
	
	if (Enemy)
	{
		PlayRandomDeathSound(Enemy);
		
		Enemy->SetIsDead(true);
		Enemy->GetCharacterMovement()->StopMovementImmediately();
		Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (AAIController* AIC = Cast<AAIController>(Enemy->GetController()))
		{
			AIC->GetBrainComponent()->StopLogic("Death");
		}
		
		float RagdollDelay = 3.0f;
		FTimerHandle RagdollTimerHandle;
		TWeakObjectPtr<UGA_EnemyDeath> WeakThis(this);
		TWeakObjectPtr<AF4EnemyBase> WeakEnemy(Enemy);
		GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, [WeakThis, WeakEnemy]()
		{
			UGA_EnemyDeath* AbilityPtr = WeakThis.Get();
			AF4EnemyBase* EnemyPtr = WeakEnemy.Get();
			if (!AbilityPtr || !EnemyPtr)
			{
				return;
			}
			AbilityPtr->EnableRagdoll(EnemyPtr);
			AF4DropItem::TryDropItem(EnemyPtr, 0.33f);
			EnemyPtr->SetLifeSpan(3.0f);
		}, RagdollDelay, false);

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_EnemyDeath::PlayRandomDeathSound(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	USoundBase* SelectedSound = SelectRandomSoundFromPool();

	if (SelectedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			Actor->GetWorld(),
			SelectedSound,
			Actor->GetActorLocation()
		);
	}
}

TObjectPtr<USoundBase> UGA_EnemyDeath::SelectRandomSoundFromPool() const
{
	if (SoundPool.Num() == 0)
	{
		return nullptr;
	}

	float TotalWeight = 0.0f;

	for (const FSoundWeightData& Data : SoundPool)
	{
		TotalWeight += Data.Weight;
	}

	if (TotalWeight <= 0.0f)
	{
		return nullptr;
	}

	const float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
	float AccumulatedWeight = 0.0f;

	for (const FSoundWeightData& Data : SoundPool)
	{
		AccumulatedWeight += Data.Weight;
		if (RandomValue <= AccumulatedWeight)
		{
			return Data.Sound;
		}
	}

	return nullptr;
}

void UGA_EnemyDeath::HandleDropItem()
{
	AF4DropItem::TryDropItem(GetAvatarActorFromActorInfo(),0.33f);
}

void UGA_EnemyDeath::EnableRagdoll(AF4EnemyBase* Enemy)
{
	if (!Enemy) return;

	USkeletalMeshComponent* Mesh = Enemy->GetMesh();
	if (Mesh)
	{
		// 애니메이션 시스템을 Off
		Mesh->SetAnimationMode(EAnimationMode::AnimationCustomMode);
		Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetSimulatePhysics(true);
	}
}
