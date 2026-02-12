// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyClass.h"

namespace F4GameplayTags
{
	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Jump, TEXT("Ability.Movement.Jump"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Sprint, TEXT("Ability.Movement.Sprint"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Roll, TEXT("Ability.Movement.Roll"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Crouch, TEXT("Ability.Movement.Crouch"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interaction_Interact, TEXT("Ability.Interaction.Interact"));
	
	// Character State 
	UE_DEFINE_GAMEPLAY_TAG(Character_State_InAir, TEXT("Character.State.InAir"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Sprinting, TEXT("Character.State.Sprinting"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, TEXT("Character.State.Rollings"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Crouching, TEXT("Character.State.Crouching"));
	
	// Weapon Stats
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Stat_BaseDamage, TEXT("Weapon.Stat.BaseDamage"));
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Stat_MaxAmmo, TEXT("Weapon.Stat.MaxAmmo"));
}
