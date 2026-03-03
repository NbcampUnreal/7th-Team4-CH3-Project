#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "F4GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeChanged, int32, Hour, int32, Minute);

UCLASS()
class F4_API AF4GameState : public AGameState
{
	GENERATED_BODY()

public:
	AF4GameState();

	virtual void Tick(float DeltaTime) override;

	bool IsDayTime() const;

	void SetTimeOfDay(float NewTimeOfDay);
	void SetTimeFlows(bool bFlows);

	FORCEINLINE float GetTimeOfDay() const { return TimeOfDay; }
	FORCEINLINE float GetDawnTime() const { return DawnTime; }
	FORCEINLINE float GetDuskTime() const { return DuskTime; }

	UPROPERTY(BlueprintAssignable, Category = "Time")
	FOnTimeChanged OnTimeChanged;

	UPROPERTY()
	int32 LocalDifficultyPhase = 0;

public:
	UPROPERTY()
	int32 DifficultyPhase = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 0.f, ClampMax = 24.f))
	float TimeOfDay = 11.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 6.f, ClampMax = 9.f))
	float DawnTime = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 16.f, ClampMax = 20.f))
	float DuskTime = 18.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	bool bTimeFlows = true;
	
	// 난이도 증가 주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float DifficultyUpdateInterval = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
	bool bUseTimeSystem = true;

protected:
	UPROPERTY()
	float SurvivalTimer = 0.f;
	float DifficultyAccumulator = 0.0f;

private:
	int32 LastHour = -1;
	int32 LastMinute = -1;
};
