#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F4WeaponActor.generated.h"

// class UF4WeaponDataAsset;
class UF4ItemFragment_Equipment;

UCLASS()
class F4_API AF4WeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AF4WeaponActor();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	// 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootScene;
	
	// 메인 메시 (근접 무기, 총몸)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MainMeshComponent;
	
	// 보조 메시 (총기 파츠용, 필요 시 활성화)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> AttachmentMeshComponent;

public:
	// 총구의 위치 반환 함수
	UFUNCTION(BlueprintPure, Category = "Weapon|Gun")
	FTransform GetMuzzleTransform() const;
	
};
