#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class USphereComponent;

UCLASS()
class F4_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner|Settings")
	TArray<TSubclassOf<class AF4EnemyBase>> EnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Settings")
	float SpawnInterval = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Settings")
	float SpawnRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Settings")
	float TraceHeight = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Settings")
	float TraceDepth = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Difficulty")
	int32 BaseMaxEnemyCount = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Difficulty")
	float IncreaseLevelTime = 120.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Difficulty")
	int32 IncreaseEnemyCount = 2;

	// 플레이어 거리체크
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Condition")
	AActor* DistanceCheckTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Condition")
	float SafeDistance = 1200.f;


	UPROPERTY()
	TArray<AActor*> SpawnedEnemies;

	FTimerHandle SpawnTimerHandle;
	
	void TrySpawnBatch();
	int32 GetCurrentMaxCount() const;
	void CleanUpList();
	bool IsTargetTooClose() const;
	
protected:
	// 에디터에서 범위를 보여줄 구체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Spawner|Visual")
	USphereComponent* VisualizeRadius;

	// 에디터에서 값이 수정될 때 실행되는 함수
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
