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
	float InterpSpeed = 15.0f;

private:
	float CurrentFOV;
};
