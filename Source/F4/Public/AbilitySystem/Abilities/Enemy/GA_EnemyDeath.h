#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_EnemyDeath.generated.h"

class AF4EnemyBase;

UCLASS()
class F4_API UGA_EnemyDeath : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_EnemyDeath();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death | DropItem")
	UDataTable* DropItemData;
	
	void HandleDropItem();
	void SpawnDropItem(const struct FItemDropData* Data) ;
	
	void EnableRagdoll(AF4EnemyBase* Enemy);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death | DropItem")
	TSubclassOf<AActor> FixDropItem;
	void SpawnDropFixItem(const TSubclassOf<AActor> FixItem) ;
};
