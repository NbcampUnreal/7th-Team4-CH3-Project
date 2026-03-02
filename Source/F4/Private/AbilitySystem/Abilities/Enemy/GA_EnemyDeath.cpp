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
		Enemy->SetIsDead(true);
		Enemy->GetCharacterMovement()->StopMovementImmediately();
		Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (AAIController* AIC = Cast<AAIController>(Enemy->GetController()))
		{
			AIC->GetBrainComponent()->StopLogic("Death");
		}
		
		float RagdollDelay = 3.0f; 
		FTimerHandle RagdollTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, [this, Enemy]()
		{
			if (Enemy)
			{
				this->EnableRagdoll(Enemy);
				HandleDropItem();
				
				// 사망 3초후 destroy
				Enemy->SetLifeSpan(3.0f);
			}
		}, RagdollDelay, false);

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
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
