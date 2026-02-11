// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTags.h"
#include "NativeGameplayTags.h"

namespace F4GameplayTags
{
	// Ability Tags 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Crouch);
	
	// Character State 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_InAir);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Rolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Crouching);
}