#include "Enviroment/DynamicSky.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Enviroment/WeatherData.h"


ADynamicSky::ADynamicSky()
{
	PrimaryActorTick.bCanEverTick = true;

	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	SunLight->SetupAttachment(RootComponent);
	SunLight->bUseTemperature = true; 
	
	MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
	MoonLight->SetupAttachment(RootComponent);
	MoonLight->bUseTemperature = true; 
	MoonLight->ForwardShadingPriority = 2; 
	
	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("USkyLight"));
	SkyLight->SetupAttachment(RootComponent);
	SkyLight->SetRealTimeCapture(true);
	
	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyAtmosphere->SetupAttachment(RootComponent);
	
	SkySphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphere"));
	SkySphereMesh->SetupAttachment(RootComponent);
	SkySphereMesh->SetWorldScale3D(FVector(1000000.f)); 
	
	ExponentialHeightFog= CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
	ExponentialHeightFog->SetupAttachment(RootComponent);
}

void ADynamicSky::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	InitializeSky();
	CalculateTargetRotation();
	UpdateSkySettings(); 
	
	SunLight->SetWorldRotation(TargetSunRotation);
	MoonLight->SetWorldRotation(TargetMoonRotation);
	
	UpdateSkySettings();
}

void ADynamicSky::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSky();
	UpdateSky();
	
	UpdateSkySettings(); 

	SunLight->SetWorldRotation(TargetSunRotation);
	MoonLight->SetWorldRotation(TargetMoonRotation);
	
	GetWorld()->GetTimerManager().SetTimer(
		SkyUpdateTimerHandle,
		this,
		&ThisClass::UpdateSky,
		SkyUpdateInterval,
		true
	);
	
}

void ADynamicSky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (SunLight->IsVisible())
	{
		FRotator NewRot = FMath::RInterpTo(
			SunLight->GetComponentRotation(), 
			TargetSunRotation,
			DeltaTime,
			InterpSpeed
		);
		
		SunLight->SetWorldRotation(NewRot);
	}
	
	if (MoonLight->IsVisible())
	{
		FRotator NewRot = FMath::RInterpTo(
			MoonLight->GetComponentRotation()
			, TargetMoonRotation, 
			DeltaTime, 
			InterpSpeed
		);
		
		MoonLight->SetWorldRotation(NewRot);
	}
}

void ADynamicSky::InitializeSky()
{
	if (SkySphereMesh->GetMaterial(0))
	{
		SkySphereDynamicMaterial = SkySphereMesh->CreateDynamicMaterialInstance(0,SkySphereMesh->GetMaterial(0));
	}
	
}

bool ADynamicSky::IsDayTime()
{
	return (TimeOfDay >= DawnTime && TimeOfDay <= DuskTime);
}

void ADynamicSky::UpdateSky()
{
	if (bTimeFlows)
	{
		UpdateTimeOfDay(SkyUpdateInterval);
		
		CalculateTargetRotation(); 
		
		int32 CurrentHour = FMath::FloorToInt(TimeOfDay);
		int32 CurrentMinute = FMath::FloorToInt((TimeOfDay - CurrentHour) * 60.0f);
		
		if (CurrentHour != LastHour || CurrentMinute != LastMinute)
		{
			LastHour = CurrentHour;
			LastMinute = CurrentMinute;

			OnTimeChanged.Broadcast(CurrentHour, CurrentMinute);
		}
	}
	
	const bool bIsNowDayTime = IsDayTime(); 
	if (bWasDayTime != bIsNowDayTime)
	{
		SunLight->SetVisibility(bIsNowDayTime);
		MoonLight->SetVisibility(!bIsNowDayTime);
		
		bWasDayTime = bIsNowDayTime;
	}
	
	UpdateSkySettings();
	UpdateExponentialHeightFogSettings();
}

void ADynamicSky::UpdateTimeOfDay(float DeltaTime)
{
	TimeOfDay += DeltaTime * TimeSpeed / 60.f;
	
	if (TimeOfDay >= 24.0f) TimeOfDay -= 24.0f;
	if (TimeOfDay < 0.0f) TimeOfDay = 24.0f;
}

void ADynamicSky::CalculateTargetRotation()
{
	if (IsDayTime())
	{
		float SunYaw =FMath::GetMappedRangeValueUnclamped(
			FVector2D(DawnTime, DuskTime - 0.2f),
			FVector2D(0.0f, -180.f),
			TimeOfDay
		);
		
		const FRotator Rotation = GetActorRotation(); 
		TargetSunRotation = FRotator(SunYaw, Rotation.Yaw, Rotation.Roll);
	}
	else
	{
		float MoonYaw; 
		if (TimeOfDay >= DuskTime && TimeOfDay <= 24.0f) 
		{ 
			MoonYaw = FMath::GetMappedRangeValueUnclamped(
				FVector2D(DuskTime, 24.0f),
				FVector2D(0.0f, -90.0f),
				TimeOfDay
			);
		}
		else
		{
			MoonYaw = FMath::GetMappedRangeValueUnclamped(
				FVector2D(0.0f, DawnTime - 0.3f),
				FVector2D(-90.0f, -180.0f),
				TimeOfDay
			);
		}
		const FRotator Rotation = GetActorRotation();
		TargetMoonRotation = FRotator(MoonYaw, Rotation.Yaw, Rotation.Roll);
	}
}

void ADynamicSky::UpdateSkySettings()
{
	if (!WeatherData) return;
	
	const bool bIsDayTime = IsDayTime();
	
	SunLight->SetVisibility(bIsDayTime);
	MoonLight->SetVisibility(!bIsDayTime);
	
	if (bIsDayTime)
	{
		bStarVisibility = false;
		bMoonVisibility = false;
		
		UpdateSunSettings();   
	}
	else
	{
		bStarVisibility = true;
		bMoonVisibility = true;
		
		UpdateMoonSettings();
	}
	
	if (SkySphereDynamicMaterial)
	{
		SkySphereDynamicMaterial->SetScalarParameterValue(FName("MoonVisibility"),bMoonVisibility ? 1.0f : 0.f );
	}
	
	UpdateStarSettings();
	UpdateSkyLightSettings();
}

void ADynamicSky::UpdateSunSettings()
{
	const FDirectionalLightSettings SunLightSettings = WeatherData->GetDirectionalLightSettings(true);
	
	float SunPitch = TargetSunRotation.Pitch;
	float SunAlpha = FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, -10.0f),
		FVector2D(0.0f, 1.0f),
		SunPitch
	);
	
	SunLight->SetIntensity(SunLightSettings.Intensity * SunAlpha);
	SunLight->SetLightColor(SunLightSettings.LightColor);
	SunLight->SetLightSourceAngle(SunLightSettings.SourceAngle);
	SunLight->SetTemperature(SunLightSettings.Temperature);
	
	SunLight->SetVisibility(SunAlpha > 0.001f);
}

void ADynamicSky::UpdateMoonSettings()
{
	const FDirectionalLightSettings MoonLightSettings = WeatherData->GetDirectionalLightSettings(false);

	MoonLight->SetIntensity(MoonLightSettings.Intensity);
	MoonLight->SetLightColor(MoonLightSettings.LightColor);
	MoonLight->SetLightSourceAngle(MoonLightSettings.SourceAngle);
	MoonLight->SetTemperature(MoonLightSettings.Temperature);
}

void ADynamicSky::UpdateStarSettings()
{
	if (!SkySphereDynamicMaterial) return;
	
	if (bStarVisibility)
	{
		SkySphereDynamicMaterial->SetScalarParameterValue("StarVisibility", 1.f);
	}
	else
	{
		SkySphereDynamicMaterial->SetScalarParameterValue("StarVisibility", 0.f);
	}
}

void ADynamicSky::UpdateSkyLightSettings()
{
	SkyLight->SetIntensity(WeatherData->GetSkyLightSettings(IsDayTime()).Intensity);
}

void ADynamicSky::UpdateExponentialHeightFogSettings()
{
	const FExponentialHeightFogSettings ExponentialHeightFogSettings = WeatherData->GetExponentialHeightFogSettings(IsDayTime());
	const FExponentialHeightFogSettings DayFog = WeatherData->GetExponentialHeightFogSettings(true);
	const FExponentialHeightFogSettings NightFog = WeatherData->GetExponentialHeightFogSettings(false);
	
	float MidDay = (DawnTime + DuskTime) / 2.0f;
	float Range = (DuskTime - DawnTime) / 2.0f;
	float Diff = FMath::Abs(TimeOfDay - MidDay);
	
	float Alpha = FMath::Clamp(1.0f - (Diff / Range), 0.0f, 1.0f);
	
	float TargetExtinction = FMath::Lerp(NightFog.ExtinctionScale, DayFog.ExtinctionScale, Alpha);
	FLinearColor TargetColor = FLinearColor::LerpUsingHSV(NightFog.EmissiveColor, DayFog.EmissiveColor, Alpha);
	
	ExponentialHeightFog->SetVolumetricFog(ExponentialHeightFogSettings.bUseVolumetricFog);
	ExponentialHeightFog->SetVolumetricFogExtinctionScale(TargetExtinction);
	ExponentialHeightFog->SetVolumetricFogEmissive(TargetColor);
}
