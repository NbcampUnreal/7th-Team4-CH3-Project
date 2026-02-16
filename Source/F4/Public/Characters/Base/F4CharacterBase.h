

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "F4CharacterBase.generated.h"

class UGameplayAbility; 
class UF4AttributeSetCharacter; 

struct FGameplayAbilitySpecHandle;
UCLASS()
class F4_API AF4CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AF4CharacterBase();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	void UpdateMoveSpeed(); 
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
#pragma region Component
	
	
#pragma endregion

	
#pragma region Gameplay Abilities 
protected:
	
	void OnSpeedAttributeChanged(const FOnAttributeChangeData& Data);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC; // 어빌리티 시스템 컴포넌트 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | Attributes")
	UF4AttributeSetCharacter* AttributeSet; // 어트리뷰트 셋 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Abilities")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities; // 기본 어빌리티 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Abilities")
	TArray<FGameplayAbilitySpecHandle> InitialAbilitySpecHandles;// 기본 어빌리티 SpecHandle 

#pragma endregion 
	

};
