#include "Characters/Player/F4PlayerCameraManager.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"

AF4PlayerCameraManager::AF4PlayerCameraManager()
{
	ViewPitchMin = -90.0f;
	ViewPitchMax = 30.0f;

	// TODO: Aim 용도인가?
	// ViewYawMin = -60.0f;
	// ViewYawMax = 60.0f;

	BaseFOV = 90.0f;
	AimingFOV = 60.0f;
	InterpSpeed = 15.0f;
	CurrentFOV = BaseFOV;
}

void AF4PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	AActor* TargetActor = OutVT.Target;
	if (!TargetActor)
	{
		return;
	}

	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(TargetActor);
	if (!ASCInterface)
	{
		return;
	}

	UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	float TargetFOV = ASC->HasMatchingGameplayTag(F4GameplayTags::State_Aiming) ? AimingFOV : BaseFOV;
	if (CurrentFOV <= 0.0f)
	{
		CurrentFOV = BaseFOV;
	}
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, InterpSpeed);

	OutVT.POV.FOV = CurrentFOV;
}
