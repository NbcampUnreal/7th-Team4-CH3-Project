#include "Characters/Player/F4PlayerCameraManager.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "System/F4GameplayTags.h"

AF4PlayerCameraManager::AF4PlayerCameraManager()
{
	ViewPitchMin = -90.0f;
	ViewPitchMax = 30.0f;

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

	bool bIsAiming = ASC->HasMatchingGameplayTag(F4GameplayTags::State_Aiming);

	float TargetFOV = bIsAiming ? AimingFOV : BaseFOV;
	if (CurrentFOV <= 0.0f)
	{
		CurrentFOV = BaseFOV;
	}
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, InterpSpeed);
	OutVT.POV.FOV = CurrentFOV;

	FVector TargetOffset = bIsAiming ? AimingOffset : BaseOffset;
	CurrentOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, InterpSpeed);

	FRotator TargetRotationOffset = bIsAiming ? AimingRotationOffset : BaseRotationOffset;
	CurrentRotationOffset = FMath::RInterpTo(CurrentRotationOffset, TargetRotationOffset, DeltaTime, InterpSpeed);

	FVector TargetLocation = TargetActor->GetActorLocation();
	FRotator TargetRotation = OutVT.POV.Rotation;

	FQuat FrameQuat = FQuat(TargetRotation);
	FVector WorldOffset = FrameQuat.RotateVector(CurrentOffset);

	OutVT.POV.Location = TargetLocation + WorldOffset;
	OutVT.POV.Rotation = TargetRotation + CurrentRotationOffset;
}
