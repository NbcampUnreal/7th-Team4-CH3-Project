#pragma once

#include "NativeGameplayTags.h"

namespace F4GameplayTags
{
	// Ability Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Crouch);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Interact);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Pickup);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Aim);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Fire);
	
	// Character State Tags 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_InAir);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Rolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Crouching);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Interacting);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Firing);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_RegenStamina);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_NoRegenStamina);

	// Weapon Stats Tags 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Stat_BaseDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Stat_MaxAmmo);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Reloading);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Reload);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Reload);
	
	//Enemy State
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attacking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Death);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_DealDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Character_Die);
	
	// Event Tags 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Hit_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Interaction_Pickup);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_Equip);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Zoom);
}
