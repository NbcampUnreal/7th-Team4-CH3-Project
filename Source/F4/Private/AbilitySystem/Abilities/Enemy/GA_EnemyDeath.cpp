#include "AbilitySystem/Abilities/Enemy/GA_EnemyDeath.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "DataTable/ItemDropData.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/F4GameplayTags.h"

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
			}
		}, RagdollDelay, false);

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_EnemyDeath::HandleDropItem()
{
	/*AActor* OwnerActor = GetAvatarActorFromActorInfo();
	if (!OwnerActor || !DropItemData)
	{
		return;
	}
	
	TArray<FName> RowNames = DropItemData->GetRowNames();
	if (RowNames.IsEmpty())
	{
		return;
	}
	
	float TotalRate =  0.f;
	TArray<FItemDropData*> ValidDrops;
	
	for (const FName& RowName : RowNames)
	{
		FItemDropData* DropData = DropItemData->FindRow<FItemDropData>(RowName, TEXT(""));
		if (DropData && DropData->DropRate > 0.f)
		{
			TotalRate+=DropData->DropRate;
			ValidDrops.Add(DropData);
		}
	}
	
	if (TotalRate <=  0.f)
	{
		return;
	}
	
	const float RandomRoll = FMath::RandRange(0.f, TotalRate);
	float CumulativeProbability = 0.f;
	
	for (const FName& RowName: RowNames)
	{
		for (FItemDropData* DropData : ValidDrops)
		{
			CumulativeProbability += DropData->DropRate;
        
			if (RandomRoll >= CumulativeProbability)
			{
				SpawnDropItem(DropData);
				break;
			}
		}
	}*/
	
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return;
	}
	
	if (FixDropItem)
	{
		SpawnDropFixItem(FixDropItem);
	}
}

void UGA_EnemyDeath::SpawnDropItem(const FItemDropData* Data)
{
	if (!Data)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	if (!World || !OwnerActor)
	{
		return;
	}
	
	TSubclassOf<AActor> ItemClass = Data->ItemClass.LoadSynchronous();
	
	if (ItemClass)
	{
		FVector SpawnLocation = OwnerActor->GetActorLocation();
       
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		World->SpawnActor<AActor>(ItemClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	}
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

void UGA_EnemyDeath::SpawnDropFixItem(const TSubclassOf<AActor> FixItem)
{
	if (!FixItem)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	if (!World || !OwnerActor)
	{
		return;
	}
	
	FVector SpawnLocation = OwnerActor->GetActorLocation();
        
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 즉시 소환
	World->SpawnActor<AActor>(FixItem, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
        
	UE_LOG(LogTemp, Log, TEXT("Fixed Item Spawned: %s"), *FixItem->GetName());
}
