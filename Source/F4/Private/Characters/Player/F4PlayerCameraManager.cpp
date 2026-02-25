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
	FVector ActorLocation = OutVT.Target ? OutVT.Target->GetActorLocation() : FVector::ZeroVector;
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
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, InterpSpeed);
	OutVT.POV.FOV = (CurrentFOV <= 0.0f) ? BaseFOV : CurrentFOV;

	FVector TargetOffset = bIsAiming ? AimingOffset : BaseOffset;
	CurrentOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, InterpSpeed);

	FRotator TargetRotationOffset = bIsAiming ? AimingRotationOffset : BaseRotationOffset;
	CurrentRotationOffset = FMath::RInterpTo(CurrentRotationOffset, TargetRotationOffset, DeltaTime, InterpSpeed);

	FVector ShakeDelta = OutVT.POV.Location - ActorLocation;

	FQuat FrameQuat = FQuat(OutVT.POV.Rotation);
	FVector WorldOffset = FrameQuat.RotateVector(CurrentOffset);

	OutVT.POV.Location = TargetActor->GetActorLocation() + WorldOffset + ShakeDelta;
	OutVT.POV.Rotation += CurrentRotationOffset;
}
