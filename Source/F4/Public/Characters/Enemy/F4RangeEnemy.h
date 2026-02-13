#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "F4RangeEnemy.generated.h"

class USceneComponent;

UCLASS()
class F4_API AF4RangeEnemy : public AF4EnemyBase
{
	GENERATED_BODY()

public:
	AF4RangeEnemy();

protected:
	// 투사체가 나갈 위치 (총구 등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "F4 | Combat")
	USceneComponent* MuzzleComponent;

public:
	// 블루프린트 GA에서 투사체 소환 위치를 가져갈 때 사용
	UFUNCTION(BlueprintCallable, Category = "F4 | Combat")
	FVector GetMuzzleLocation() const { return MuzzleComponent->GetComponentLocation(); }
};
