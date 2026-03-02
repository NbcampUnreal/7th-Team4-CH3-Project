#include "Enviroment/DynamicSky.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Enviroment/WeatherData.h"
#include "System/F4GameState.h"


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

	ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
	ExponentialHeightFog->SetupAttachment(RootComponent);
}

void ADynamicSky::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 에디터에서는 GameState가 없으므로 Preview 값으로 렌더링
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

	CachedGameState = GetWorld()->GetGameState<AF4GameState>();

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
			MoonLight->GetComponentRotation(),
			TargetMoonRotation,
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
		SkySphereDynamicMaterial = SkySphereMesh->CreateDynamicMaterialInstance(0, SkySphereMesh->GetMaterial(0));
	}
}

float ADynamicSky::GetEffectiveTimeOfDay() const
{
	return CachedGameState ? CachedGameState->GetTimeOfDay() : PreviewTimeOfDay;
}

float ADynamicSky::GetEffectiveDawnTime() const
{
	return CachedGameState ? CachedGameState->GetDawnTime() : PreviewDawnTime;
}

float ADynamicSky::GetEffectiveDuskTime() const
{
	return CachedGameState ? CachedGameState->GetDuskTime() : PreviewDuskTime;
}

bool ADynamicSky::IsDayTime() const
{
	const float TimeOfDay = GetEffectiveTimeOfDay();
	return (TimeOfDay >= GetEffectiveDawnTime() && TimeOfDay <= GetEffectiveDuskTime());
}

float ADynamicSky::CalculateDayNightBlend() const
{
	const float TimeOfDay = GetEffectiveTimeOfDay();
	const float DawnTime = GetEffectiveDawnTime();
	const float DuskTime = GetEffectiveDuskTime();

	// 새벽 전환 구간: 서서히 밝아짐
	if (TimeOfDay >= DawnTime - TransitionHours && TimeOfDay <= DawnTime + TransitionHours)
	{
		return FMath::SmoothStep(DawnTime - TransitionHours, DawnTime + TransitionHours, TimeOfDay);
	}
	// 황혼 전환 구간: 서서히 어두워짐
	if (TimeOfDay >= DuskTime - TransitionHours && TimeOfDay <= DuskTime + TransitionHours)
	{
		return 1.0f - FMath::SmoothStep(DuskTime - TransitionHours, DuskTime + TransitionHours, TimeOfDay);
	}
	// 완전한 낮
	if (TimeOfDay > DawnTime && TimeOfDay < DuskTime)
	{
		return 1.0f;
	}
	// 완전한 밤
	return 0.0f;
}

void ADynamicSky::UpdateSky()
{
	if (!CachedGameState)
	{
		CachedGameState = GetWorld()->GetGameState<AF4GameState>();
	}

	CalculateTargetRotation();
	UpdateSkySettings();
	UpdateExponentialHeightFogSettings();
}

void ADynamicSky::CalculateTargetRotation()
{
	const float TimeOfDay = GetEffectiveTimeOfDay();
	const float DawnTime  = GetEffectiveDawnTime();
	const float DuskTime  = GetEffectiveDuskTime();

	if (IsDayTime())
	{
		float SunYaw = FMath::GetMappedRangeValueUnclamped(
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
	if (!WeatherData)
	{
		return;
	}

	const float DayBlend = CalculateDayNightBlend();

	UpdateSunSettings(DayBlend);
	UpdateMoonSettings(DayBlend);

	if (SkySphereDynamicMaterial)
	{
		SkySphereDynamicMaterial->SetScalarParameterValue(FName("MoonVisibility"), 1.0f - DayBlend);
	}

	UpdateStarSettings();
	UpdateSkyLightSettings();
}

void ADynamicSky::UpdateSunSettings(float DayBlend)
{
	const FDirectionalLightSettings SunLightSettings = WeatherData->GetDirectionalLightSettings(true);

	float SunPitch = TargetSunRotation.Pitch;
	float SunPitchAlpha = FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, -10.0f),
		FVector2D(0.0f, 1.0f),
		SunPitch
	);

	float FinalAlpha = SunPitchAlpha * DayBlend;

	SunLight->SetIntensity(SunLightSettings.Intensity * FinalAlpha);
	SunLight->SetLightColor(SunLightSettings.LightColor);
	SunLight->SetLightSourceAngle(SunLightSettings.SourceAngle);
	SunLight->SetTemperature(SunLightSettings.Temperature);

	SunLight->SetVisibility(FinalAlpha > 0.001f);
}

void ADynamicSky::UpdateMoonSettings(float DayBlend)
{
	const FDirectionalLightSettings MoonLightSettings = WeatherData->GetDirectionalLightSettings(false);

	float NightBlend = 1.0f - DayBlend;

	MoonLight->SetIntensity(MoonLightSettings.Intensity * NightBlend);
	MoonLight->SetLightColor(MoonLightSettings.LightColor);
	MoonLight->SetLightSourceAngle(MoonLightSettings.SourceAngle);
	MoonLight->SetTemperature(MoonLightSettings.Temperature);

	MoonLight->SetVisibility(NightBlend > 0.001f);
}

void ADynamicSky::UpdateStarSettings()
{
	if (!SkySphereDynamicMaterial)
	{
		return;
	}

	SkySphereDynamicMaterial->SetScalarParameterValue("StarVisibility", 1.0f - CalculateDayNightBlend());
}

void ADynamicSky::UpdateSkyLightSettings()
{
	const float DayBlend = CalculateDayNightBlend();
	const float DayIntensity = WeatherData->GetSkyLightSettings(true).Intensity;
	const float NightIntensity = WeatherData->GetSkyLightSettings(false).Intensity;
	SkyLight->SetIntensity(FMath::Lerp(NightIntensity, DayIntensity, DayBlend));
}

void ADynamicSky::UpdateExponentialHeightFogSettings()
{
	const float TimeOfDay = GetEffectiveTimeOfDay();
	const float DawnTime  = GetEffectiveDawnTime();
	const float DuskTime  = GetEffectiveDuskTime();

	const FExponentialHeightFogSettings DayFog = WeatherData->GetExponentialHeightFogSettings(true);
	const FExponentialHeightFogSettings NightFog = WeatherData->GetExponentialHeightFogSettings(false);

	float MidDay = (DawnTime + DuskTime) / 2.0f;
	float Range = (DuskTime - DawnTime) / 2.0f;
	float Diff = FMath::Abs(TimeOfDay - MidDay);
	float Alpha = FMath::Clamp(1.0f - (Diff / Range), 0.0f, 1.0f);

	float TargetExtinction = FMath::Lerp(NightFog.ExtinctionScale, DayFog.ExtinctionScale, Alpha);
	FLinearColor TargetColor = FLinearColor::LerpUsingHSV(NightFog.EmissiveColor, DayFog.EmissiveColor, Alpha);

	const FExponentialHeightFogSettings CurrentFog = WeatherData->GetExponentialHeightFogSettings(IsDayTime());
	ExponentialHeightFog->SetVolumetricFog(CurrentFog.bUseVolumetricFog);
	ExponentialHeightFog->SetVolumetricFogExtinctionScale(TargetExtinction);
	ExponentialHeightFog->SetVolumetricFogEmissive(TargetColor);
}
