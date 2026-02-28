#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicSky.generated.h"

class UExponentialHeightFogComponent;
class UDirectionalLightComponent; 

class UWeatherData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeChanged, int32, Hour, int32, Minute);

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
	
	bool IsDayTime();
	
protected:
	
	void UpdateSky();
	
	void UpdateTimeOfDay(float DeltaTime);
	
	void CalculateTargetRotation();
	
	void UpdateSkySettings();
	
	void UpdateSunSettings();
	
	void UpdateMoonSettings();
	
	void UpdateStarSettings();
	
	void UpdateSkyLightSettings();
	
	void UpdateExponentialHeightFogSettings();
	
public:
	
	FORCEINLINE float GetDawnTime() const {return DawnTime;}
	FORCEINLINE float GetDuskTime() const {return DuskTime;}
	
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
	
	bool bTimeFlows = true;
	
	bool bWasDayTime = true; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 0.f, ClampMax = 24.f))
	float TimeOfDay = 11.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 6.f, ClampMax = 9.f))
	float DawnTime = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 16.f, ClampMax = 20.f))
	float DuskTime = 18.0f;
	
	int32 LastHour = -1;
	int32 LastMinute = -1;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Settings")
	UWeatherData* WeatherData; 
protected:
	
	bool bStarVisibility = false;
	bool bMoonVisibility = false;
	
public:
	FOnTimeChanged OnTimeChanged;
};
