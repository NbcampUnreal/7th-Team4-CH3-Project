#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SendEvent.generated.h"

UCLASS()
class F4_API UAN_SendEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAN_SendEvent();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventTags")
	TArray<FGameplayTag> EventTags;
};
