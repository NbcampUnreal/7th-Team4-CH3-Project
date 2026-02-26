#include "System/F4GameplayTags.h"

namespace F4GameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(State_Firing, TEXT("State.Firing"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Fire, TEXT("Ability.Combat.Fire"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Fire, TEXT("Event.Montage.Fire"));

	UE_DEFINE_GAMEPLAY_TAG(State_Aiming, TEXT("State.Aiming"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Aim, TEXT("Ability.Combat.Aim"));
    UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Equip, TEXT("Ability.Combat.Equip"));

	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Jump, TEXT("Ability.Movement.Jump"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Sprint, TEXT("Ability.Movement.Sprint"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Roll, TEXT("Ability.Movement.Roll"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Crouch, TEXT("Ability.Movement.Crouch"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interaction_Interact, TEXT("Ability.Interaction.Interact"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interaction_Pickup, TEXT("Ability.Interaction.Pickup"));

	// Character State
	UE_DEFINE_GAMEPLAY_TAG(Character_State_InAir, TEXT("Character.State.InAir"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Sprinting, TEXT("Character.State.Sprinting"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, TEXT("Character.State.Rollings"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Movement_Crouching, TEXT("Character.State.Crouching"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Interacting, TEXT("Character.State.Interacting"));
	
	
	UE_DEFINE_GAMEPLAY_TAG(Character_State_RegenStamina, TEXT("Character.State.RegenStamina"));
	UE_DEFINE_GAMEPLAY_TAG(Character_State_NoRegenStamina, TEXT("Character.State.NoRegenStamina"));
	
	UE_DEFINE_GAMEPLAY_TAG(State_Invincible, TEXT("State.Invincible"));

	// Weapon Stats
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Stat_BaseDamage, TEXT("Weapon.Stat.BaseDamage"));
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Stat_MaxAmmo, TEXT("Weapon.Stat.MaxAmmo"));

	UE_DEFINE_GAMEPLAY_TAG(State_Reloading, TEXT("State.Reloading"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Reload, TEXT("Ability.Combat.Reload"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Reload, TEXT("Event.Montage.Reload"));

	// Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Event_Hit_Damage, TEXT("Event.Hit.Damage"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Interaction_Pickup, TEXT("Event.Interaction.Pickup"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Equip, TEXT("Event.Combat.Equip"));

	// Enemy State
	UE_DEFINE_GAMEPLAY_TAG(State_Attacking, TEXT("State.Attacking"));
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, TEXT("State.Dead"));
	UE_DEFINE_GAMEPLAY_TAG(State_Chasing, TEXT("State.Chasing"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack, TEXT("Ability.Attack"));
	UE_DEFINE_GAMEPLAY_TAG(Ability_Death, TEXT("Ability.Death"));
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Attack, TEXT("Cooldown.Attack"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_DealDamage, TEXT("Event.Montage.DealDamage"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Character_Die, TEXT("Event.Character.Die"));
	
	
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Zoom, "InputTag.Zoom");
	
}
