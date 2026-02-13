#pragma once

#include "NativeGameplayTags.h"

namespace F4GameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Firing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Fire);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Aim);

	// Ability Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Crouch);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Interact);

	// Character State
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_InAir);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Rolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Crouching);

	// Weapon Stats
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Stat_BaseDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Stat_MaxAmmo);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Reloading);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Reload);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Reload);
}
