#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_EnemySkillEffect.generated.h"

class UNiagaraSystem;

UCLASS()
class F4_API UANS_EnemySkillEffect : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UANS_EnemySkillEffect();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Niagara")
	TObjectPtr<UNiagaraSystem> Template;
	
	UPROPERTY(EditAnywhere, Category = "Niagara")
	FVector LocationOffset = FVector(0.f, 0.f, 2.0f);
	
	UPROPERTY(EditAnywhere, Category = "Niagara")
	bool bUseCustomRotation = false;

	UPROPERTY(EditAnywhere, Category = "Niagara", meta = (EditCondition = "bUseCustomRotation"))
	FRotator CustomRotation = FRotator::ZeroRotator;

private:
	UPROPERTY()
	TMap<USkeletalMeshComponent*, class UNiagaraComponent*> ActiveComponents;
};
