#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F4WeaponActor.generated.h"

class UF4WeaponDataAsset;

UCLASS()
class F4_API AF4WeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AF4WeaponActor();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	// 무기 데이터 에셋 주입 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitializeWeapon(const UF4WeaponDataAsset* InWeaponData);

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
	
	// 현재 할당된 데이터
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<const UF4WeaponDataAsset> WeaponData;
	
public:
	// 총구의 위치 반환 함수
	UFUNCTION(BlueprintPure, Category = "Weapon|Gun")
	FTransform GetMuzzleTransform() const;
};
