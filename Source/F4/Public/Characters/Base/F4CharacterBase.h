

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/F4AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "F4CharacterBase.generated.h"

class UF4AttributeSetWeapon;
class UGameplayAbility;
class UF4AttributeSetCharacter; 

struct FGameplayAbilitySpecHandle;
UCLASS()
class F4_API AF4CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AF4CharacterBase();
	
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	void InitAbilitySystem(); 
	
	void InitializeAttributes(); 
	
	void OnSpeedChanged(const FOnAttributeChangeData& Data);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UF4AbilitySystemComponent> ASC; // 어빌리티 시스템 컴포넌트 
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "GAS | Attributes")
	TObjectPtr<UF4AttributeSetCharacter> AttributeSet; // 어트리뷰트 셋

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "GAS | Attributes")
	TObjectPtr<UF4AttributeSetWeapon> WeaponAttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Abilities")
	TArray<FGameplayAbilitySpecHandle> InitialAbilitySpecHandles;// 기본 어빌리티 SpecHandle 
	
};
