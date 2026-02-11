// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Movement/GA_Jump.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "DummyClass.h"


UGA_Jump::UGA_Jump()
{
	FGameplayTagContainer ActivationInfo;
	ActivationInfo.AddTag(F4GameplayTags::Ability_Movement_Jump); 
	SetAssetTags(ActivationInfo); 
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AActor* Actor = GetAvatarActorFromActorInfo();
	if (Actor == nullptr) return; 
	
	ACharacter* Character = Cast<ACharacter>(Actor);
	if (Character == nullptr) return; 
	
	if (CommitAbility(Handle,ActorInfo,ActivationInfo) == false) return;
	
	Character->ACharacter::Jump();
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true ,true); 
	
}
