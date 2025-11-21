// Qhax's GAS Template for SinglePlayer

#pragma once

#include "NativeGameplayTags.h"


namespace GAS_Tags
{
	/************************************************
	*----------------<Gameplay Tags>----------------*
	* Add all of the Gameplay related tags to here. *
	*************************************************/

	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmune);


	// ============================================================
	// ATTRIBUTE TAGS
	// ============================================================
	// Health
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Health);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Health_Full);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Health_Regen);

	// Mana
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Mana);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Mana_Full);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Mana_Regen);

	// Posture
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Posture);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Posture_Full);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Posture_Empty);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Attribute_Posture_Regen);


	// ============================================================
    // EVENTS
    // ============================================================
    // Trigger Event
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_Trigger_Perfect);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_Trigger_Perfect_Dodge);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_Trigger_Perfect_Parry);
	
	// Attack Notifies
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_Attack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_Attack_TraceStart);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_Attack_TraceEnd);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_Attack_CanActivateNextAttack);

	// Movement & Rotation Notifies
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_MotionWarping);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_LockRotationTowardsTarget);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_UnLockRotationTowardsTarget);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_Hero_LockRotation);

	// Weapon Notifies
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_Weapon_StorePreviousLocation);

	// Misc Notifies
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Event_AnimNotify_Footstep);


	// ============================================================
	// ABILITY TRIGGER EVENTS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityTriggerEvent_TakeDamage);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityTriggerEvent_Death_Basic);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityTriggerEvent_Death_Finisher);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityTriggerEvent_ComboMelee);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityTriggerEvent_ParryKnockback);


	// ============================================================
	// STATE TAGS
	// ============================================================
	// These tags represent temporary gameplay states applied to the actor.
	// They can be granted by either abilities or gameplay effects, and are
	// typically added to the ASC's owned tags during execution or effect
	// application, then removed when the source ends or expires.

	// Base State
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InAir);

	// Debuff States
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Debuff);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Debuff_Stun);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Debuff_Slow);

	// Movement States
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Moving);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Moving_Strafing);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Moving_Patrolling);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Moving_Walking);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Moving_Jogging);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Moving_Running);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_Moving_Dash);

	// Combat States
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_MeleeAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_MeleeCombo);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_MeleeCombo1);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_MeleeCombo2);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_MeleeCombo3);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_AirKick);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_UnparryableAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_UnDodgebleAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_UnstoppableAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_CanInterruptUnstoppableAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_TakeDamage);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_Parry);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_ParryKnockback);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_Vulnerable);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_CanActivateFinisher);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_Finisher);

	// Death States
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_Dead);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_Dead_Basic);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_InCombat_Dead_Finisher);

	// Targeting States
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_AbilityTargeting);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_AbilityTargeting_Shadow);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_TargetLockSystem_Enemy_Targeted);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_State_LockRotationTowardsTarget);


	// ============================================================
	// WINDOW TAGS
	// ============================================================
	// Timing windows for gameplay mechanics. These tags define specific
	// time frames where certain actions become available, more effective,
	// or trigger special responses.

	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Window);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Window_Perfect);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Window_Perfect_Dodge);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Window_Perfect_Parry);


	// ============================================================
	// ABILITY TAGS
	// ============================================================
	// These tags uniquely identify specific gameplay abilities.
	// They are primarily used to classify, query, and activate abilities
	// (e.g., attack, parry, take damage) through the Gameplay Ability System.
	// Each ability owns a defining tag under the "Ability" category, which
	// serves as a stable identifier rather than a temporary state indicator.

	// Base Combat Abilities
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_TakeDamage);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Parry);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Knocback);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack);

	// Shadow Attacks
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Shadow);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Shadow_Cooldown);

	// Melee Combo Attacks
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_MeleeCombo);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_MeleeCombo_ShadowLinked);

	// Melee Finishers
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_1);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_2);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_3);

	// Skill Attacks
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Skill);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Skill_BladeStep);

	// Attack Direction Tags
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Direction);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Direction_RightToLeft);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Direction_LeftToRight);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Direction_BottomToTop);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Direction_TopToBottom);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Direction_Forward);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Direction_Backward);

	// Attack Type Tags
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type_Light);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type_Heavy);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type_Unparryable);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type_Unstoppable);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Combat_Attack_Type_CanInterruptUnstoppable);

	// Movement Abilities
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Movement);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Movement_Strafing);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Movement_Dash);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Ability_Movement_Dash_Cooldown);


	// ============================================================
	// GAMEPLAY EFFECT DATA
	// ============================================================
	// SetByCaller Tags
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_EffectData_SetByCaller_DamageAmount);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_EffectData_SetByCaller_GainHealthAmount);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_EffectData_SetByCaller_GainManaAmount);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_EffectData_SetByCaller_PostureRegenAmount);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_EffectData_SetByCaller_PostureDecreaseAmount);

	// Effect Modifiers
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_EffectData_EnableLifeSteal);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_EffectData_EnableCriticalDamage);


	// ============================================================
	// ENTITY TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Entity_Character_Hero);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Entity_Character_Enemy);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Entity_Character_Enemy_Training);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Entity_Character_Enemy_Crowd);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Entity_Character_Enemy_Boss);


	// ============================================================
	// HERO ABILITIES
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_HeroAbility0);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_HeroAbility1);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_HeroAbility2);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_HeroAbility3);


	// ============================================================
	// DIRECTION TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_Forward);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_Backward);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_Left);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_Right);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_ForwardLeft);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_ForwardRight);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_BackwardLeft);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Direction_BackwardRight);


	// ============================================================
	// TUTORIAL TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Tutorial_Dash);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Tutorial_MeleeCombo);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Tutorial_TargetLock);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Tutorial_ShadowAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Tutorial_ShadowFinisher);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Tutorial_Parry);



	/**********************************************
	*----------------<Filter Tags>----------------*
	* Add all of the Filter related tags to here. *
	***********************************************/

	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Filter);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Filter_Team);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Filter_Team_Hostile);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Filter_Team_Friend);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Filter_Team_Neutral);



	/***************************************************
	*----------------<GameplayCue Tags>----------------*
	* Add all of the GameplayCue related tags to here. *
	****************************************************/

	// ============================================================
	// ATTACHED EFFECT TEXT CUES
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_AttachedEffectText_PhysicalDamage);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_AttachedEffectText_CriticalDamage);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_AttachedEffectText_Bleed);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_AttachedEffectText_GainHealth);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_AttachedEffectText_GainMana);


	// ============================================================
	// HERO MESSAGE HUD CUES
	// ============================================================
	// Status Messages
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_LevelUp);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_Died);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_Stunned);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_Slowed);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_GainPhysicalArmor);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_GainLifeSteal);

	// Tutorial Messages
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_Tutorial_QuestCompleted);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HeroMessageHUD_Tutorial_TutorialCompleted);


	// ============================================================
	// VISUAL EFFECTS (VFX) CUES
	// ============================================================
	// Blood Effects - Slash
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Slash);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Slash_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Slash_Hero_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Slash_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Slash_Hero_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Slash_High);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Slash_Hero_High);

	// Blood Effects - Splash
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splash);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splash_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splash_Hero_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splash_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splash_Hero_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splash_High);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splash_Hero_High);

	// Blood Effects - Splatter
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splatter);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splatter_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splatter_Hero_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splatter_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splatter_Hero_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splatter_High);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Splatter_Hero_High);

	// Blood Effects - Stab
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Stab);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Stab_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Stab_Hero_Low);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Stab_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Stab_Hero_Mid);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Stab_High);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Blood_Stab_Hero_High);

	// Special VFX
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Parry);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Heal);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Hero_Ghost);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Boss_Ghost);

	// Dissolve VFX
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Dissolve_CharacterDead);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Dissolve_SpawnShadow_Hero);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_VFX_Dissolve_SpawnShadow_Boss);


	// ============================================================
	// SOUND CUES
	// ============================================================
	// Play On Target
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnTarget);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnTarget_TakeDamage);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnTarget_Finisher);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnTarget_Kick);

	// Play On Source
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnSource);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnSource_Footstep);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnSource_ParryKnocback);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnSource_AttackSwing);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnSource_Death);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnSource_LevelUp);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Sound_PlayOnSource_Kick);


	// ============================================================
	// COMBAT CAMERA & EFFECTS CUES
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Combat_Camera_Shake);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Combat_Camera_ZoomIn);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Combat_Camera_ZoomOut);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Combat_HitPasue);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Combat_HologramSlowMo);



	/************************************************
	*-------------------<AI Tags>-------------------*
	* Add all of the AI related tags to here.       *
	*************************************************/

	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI);


	// ============================================================
	// AI STATE TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_Patrolling);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_Movement);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_Attack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_InComingAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_Vulnerable);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_BackupReaction);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_HeroFinisher);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_State_IsAttackIntender);


	// ============================================================
	// AI ABILITY TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_PhantomDash);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_PhantomDash_Cooldown);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_TwinSlashUp);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_TwinSlashUp_Cooldown);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_LineBreaker);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_LineBreaker_Cooldown);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_ShadowAttack);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_ShadowAttack_Cooldown);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_Dash);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Ability_Dash_Cooldown);


	// ============================================================
	// AI ABILITY TRIGGER EVENTS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_AbilityTriggerEvent_Movement);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_AbilityTriggerEvent_Movement_Patrolling);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_AbilityTriggerEvent_Movement_Strafing_Orbit);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_AbilityTriggerEvent_Movement_Strafing_AttackIntend);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_AbilityTriggerEvent_Movement_ChaseTarget);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_AbilityTriggerEvent_Movement_Dash);


	// ============================================================
	// AI DIRECTION TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Resolved);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Resolved_Forward);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Resolved_Backward);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Resolved_Left);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Resolved_Right);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Policy);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Policy_Random);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AI_Direction_Policy_PlayerLastDirection);



	/************************************************
	*-------------------<UI Tags>-------------------*
	* Add all of the UI related tags to here.       *
	*************************************************/

	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI);


	// ============================================================
	// UI HIT TYPE TEXT TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_HitTypeText_Critical);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_HitTypeText_Blind);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_HitTypeText_Physical);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_HitTypeText_Magical);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_HitTypeText_Heal);


	// ============================================================
	// UI COOLDOWN TAGS
	// ============================================================
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Cooldown_Ability0);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Cooldown_Ability1);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Cooldown_Ability2);
	GAS_TEMPLATESP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Cooldown_Ability3);
}