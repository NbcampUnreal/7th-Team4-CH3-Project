#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "F4PlayerCameraManager.generated.h"

UCLASS()
class F4_API AF4PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AF4PlayerCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera|Aiming")
	float BaseFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Aiming")
	float AimingFOV = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Aiming")
	float InterpSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Offset")
	FVector BaseOffset = FVector(-250.0f, 0, 20.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Offset")
	FRotator BaseRotationOffset = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Offset")
	FVector AimingOffset = FVector(-250.0f, 130.0f, 20.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Offset")
	FRotator AimingRotationOffset = FRotator(0.0f, -13.0f, 0.0f);


private:
	float CurrentFOV;
	FVector CurrentOffset;
	FRotator CurrentRotationOffset;
};
