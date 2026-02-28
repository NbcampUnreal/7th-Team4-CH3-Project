

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeatherData.generated.h"

USTRUCT(BlueprintType)
struct FDirectionalLightSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor = FLinearColor::White; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SourceAngle = 0.537f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Temperature = 6500.f;
};

USTRUCT(BlueprintType)
struct FSkyLightSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity = 1.f;
};

USTRUCT(BlueprintType)
struct FSkyAtmosphereSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MultiScattering = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor RayleightScattering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MieSactteringScale = 0.003996f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MieAbsorptionScale = 0.000444f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MieAnisotropy = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AerialPerspectiveViewDistanceScale = 1.0f;
};

USTRUCT(BlueprintType)
struct FExponentialHeightFogSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseVolumetricFog = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor EmissiveColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExtinctionScale = 0.5f;
};

UCLASS()
class F4_API UWeatherData : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time")
	FDirectionalLightSettings DayTimeDirectionalLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time")
	FSkyLightSettings DayTimeSkyLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time")
	FSkyAtmosphereSettings DayTimeSkyAtmosphereSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time")
	FExponentialHeightFogSettings DayTimeExponentialHeightFogSettings;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time")
	FDirectionalLightSettings NightTimeDirectionalLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time")
	FSkyLightSettings NightTimeSkyLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time")
	FSkyAtmosphereSettings NightTimeSkyAtmosphereSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time")
	FExponentialHeightFogSettings NightTimeExponentialHeightFogSettings;
	
public:
	FORCEINLINE const FDirectionalLightSettings& GetDirectionalLightSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeDirectionalLightSettings : NightTimeDirectionalLightSettings; }
	FORCEINLINE const FSkyLightSettings& GetSkyLightSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeSkyLightSettings : NightTimeSkyLightSettings; }

	FORCEINLINE const FSkyAtmosphereSettings& GetSkyAtmosphereSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeSkyAtmosphereSettings : NightTimeSkyAtmosphereSettings; }
	FORCEINLINE const FExponentialHeightFogSettings& GetExponentialHeightFogSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeExponentialHeightFogSettings : NightTimeExponentialHeightFogSettings; }

};
