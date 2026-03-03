#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F4BuffComponent.generated.h"

class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuffAppliedSignature, float, Duration, UTexture2D*, BuffIcon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class F4_API UF4BuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UF4BuffComponent();

	UPROPERTY(BlueprintAssignable, Category = "Buff|UI")
	FOnBuffAppliedSignature OnBuffApplied;
	
	UFUNCTION(BlueprintCallable, Category = "Buff|UI")
	void AddBuffToUI(float Duration, UTexture2D* BuffIcon);
};
