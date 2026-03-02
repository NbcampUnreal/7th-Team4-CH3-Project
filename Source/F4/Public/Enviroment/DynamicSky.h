#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicSky.generated.h"

class UExponentialHeightFogComponent;
class UDirectionalLightComponent;
class UWeatherData;
class AF4GameState;

UCLASS()
class F4_API ADynamicSky : public AActor
{
	GENERATED_BODY()

public:
	ADynamicSky();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void InitializeSky();

protected:
	void UpdateSky();

	void CalculateTargetRotation();

	void UpdateSkySettings();

	void UpdateSunSettings();

	void UpdateMoonSettings();

	void UpdateStarSettings();

	void UpdateSkyLightSettings();

	void UpdateExponentialHeightFogSettings();

private:
	bool IsDayTime() const;

	// GameState가 없는 에디터 OnConstruction에서 사용하는 헬퍼
	float GetEffectiveTimeOfDay() const;
	float GetEffectiveDawnTime() const;
	float GetEffectiveDuskTime() const;

private:
	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UDirectionalLightComponent> SunLight;

	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UDirectionalLightComponent> MoonLight;

	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> SkySphereMesh;

	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphere;

	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkyLightComponent> SkyLight;

	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFog;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkySphere")
	TObjectPtr<UMaterialInterface> SkySphereMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> SkySphereDynamicMaterial;

protected:
	FRotator TargetSunRotation;
	FRotator TargetMoonRotation;

	float InterpSpeed = 1.0f;

protected:
	FTimerHandle SkyUpdateTimerHandle;
	const float SkyUpdateInterval = 0.1f;

	bool bWasDayTime = true;
	bool bStarVisibility = false;
	bool bMoonVisibility = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Settings")
	UWeatherData* WeatherData;

	// 에디터 프리뷰 전용 (런타임에는 GameState 값을 사용)
	UPROPERTY(EditAnywhere, Category = "Preview")
	float PreviewTimeOfDay = 11.0f;

	UPROPERTY(EditAnywhere, Category = "Preview")
	float PreviewDawnTime = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Preview")
	float PreviewDuskTime = 18.0f;

private:
	UPROPERTY()
	TObjectPtr<AF4GameState> CachedGameState;
};
