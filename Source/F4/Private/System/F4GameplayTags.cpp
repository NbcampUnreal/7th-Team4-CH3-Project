#include "System/F4GameplayTags.h"

namespace F4GameplayTags
{
	// 피아식별 Tags
	UE_DEFINE_GAMEPLAY_TAG(Character_Player, TEXT("Character.Player"));
	UE_DEFINE_GAMEPLAY_TAG(Character_Enemy, TEXT("Character.Enemy"));
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Invisible, TEXT("Ability.Combat.Invisible"));

	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Jump, TEXT("Ability.Movement.Jump"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Sprint, TEXT("Ability.Movement.Sprint"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Roll, TEXT("Ability.Movement.Roll"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Crouch, TEXT("Ability.Movement.Crouch"));
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interaction_Interact, TEXT("Ability.Interaction.Interact"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interaction_Pickup, TEXT("Ability.Interaction.Pickup"));   
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Equip, TEXT("Ability.Combat.Equip"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Aim, TEXT("Ability.Combat.Aim"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Fire, TEXT("Ability.Combat.Fire"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Melee, TEXT("Ability.Combat.Melee"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_HurricaneKick,TEXT("Ability.Combat.HurricaneKick"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_SwitchWeapon,TEXT("Ability.Combat.SwitchWeapon"));

	// Character State
	UE_DEFINE_GAMEPLAY_TAG(Character_State_InAir, TEXT("Character.State.InAir"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Sprinting, TEXT("Character.State.Sprinting"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, TEXT("Character.State.Rollings"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Crouching, TEXT("Character.State.Crouching"));
	
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Interacting, TEXT("Character.State.Interacting"));
	
	UE_DEFINE_GAMEPLAY_TAG(State_Aiming, TEXT("State.Aiming"));
	UE_DEFINE_GAMEPLAY_TAG(State_Firing, TEXT("State.Firing"));
	
	UE_DEFINE_GAMEPLAY_TAG(Character_State_HurricaneKicking, TEXT("Character.State.HurricaneKicking"));
	
	
	UE_DEFINE_GAMEPLAY_TAG(Character_State_RegenStamina, TEXT("Character.State.RegenStamina"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_NoRegenStamina, TEXT("Character.State.NoRegenStamina"));
	
	UE_DEFINE_GAMEPLAY_TAG(State_Invincible, TEXT("State.Invincible"));
	UE_DEFINE_GAMEPLAY_TAG(State_Switching_Weapon, TEXT("State.Switching.Weapon"));

	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Switching, TEXT("Event.Montage.Switching"));

	// Weapon Stats
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Stat_BaseDamage, TEXT("Weapon.Stat.BaseDamage"));
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Stat_MaxAmmo, TEXT("Weapon.Stat.MaxAmmo"));
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Ammo_Loaded, TEXT("Weapon.Ammo.Loaded"));

	UE_DEFINE_GAMEPLAY_TAG(State_Reloading, TEXT("State.Reloading"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Reload, TEXT("Ability.Combat.Reload"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Reload, TEXT("Event.Montage.Reload"));

	// Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Event_Hit_Damage, TEXT("Event.Hit.Damage"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Interaction_Pickup, TEXT("Event.Interaction.Pickup"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Equip, TEXT("Event.Combat.Equip"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Weapon_Switch, TEXT("Event.Weapon.Switch"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Trigger_Invisible, TEXT("Event.Trigger.Invisible"));

	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Fire, TEXT("Event.Montage.Fire"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Stamina_Exhuast, TEXT("Event.Stamina.Exhaust")); 
	
	// Enemy State
	UE_DEFINE_GAMEPLAY_TAG(State_Attacking, TEXT("State.Attacking"));
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, TEXT("State.Dead"));
	UE_DEFINE_GAMEPLAY_TAG(State_Chasing, TEXT("State.Chasing"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack, TEXT("Ability.Attack"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Death, TEXT("Ability.Death"));
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Attack, TEXT("Cooldown.Attack"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_DealDamage, TEXT("Event.Montage.DealDamage"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Character_Die, TEXT("Event.Character.Die"));
	
	// Gameplay Cue 
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_HurricaneKick, "GameplayCue.HurricaneKick");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Zoom, "InputTag.Zoom");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot, "InputTag.QuickSlot");

	// Data Value Tag
	UE_DEFINE_GAMEPLAY_TAG(Data_Reload_Ammo, "Data.Reload.Ammo");
}
