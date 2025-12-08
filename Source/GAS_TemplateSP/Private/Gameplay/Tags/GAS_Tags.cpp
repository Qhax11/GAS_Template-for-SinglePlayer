// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Tags/GAS_Tags.h"

namespace GAS_Tags
{
	/************************************************
	*----------------<Gameplay Tags>----------------*
	* Add all of the Gameplay related tags to here. *
	*************************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay, "Gameplay");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmune, "Gameplay.DamageImmune");


	// ============================================================
	// ATTRIBUTE TAGS
	// ============================================================
	// Health
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Health, "Gameplay.Attribute.Health");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Health_Full, "Gameplay.Attribute.Health.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Health_Regen, "Gameplay.Attribute.Health.Regen");

	// Mana
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Mana, "Gameplay.Attribute.Mana");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Mana_Full, "Gameplay.Attribute.Mana.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Mana_Regen, "Gameplay.Attribute.Mana.Regen");

	// Posture
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Posture, "Gameplay.Attribute.Posture");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Posture_Full, "Gameplay.Attribute.Posture.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Posture_Empty, "Gameplay.Attribute.Posture.Empty");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Posture_Regen, "Gameplay.Attribute.Posture.Regen");


	// ============================================================
    // EVENTS
    // ============================================================
    // Trigger Event
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_Trigger_Perfect, "Gameplay.Event.Trigger.Perfect");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_Trigger_Perfect_Dodge, "Gameplay.Event.Trigger.Perfect.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_Trigger_Perfect_Parry, "Gameplay.Event.Trigger.Perfect.Parry");
	
	// Attack Notifies
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Attack, "Gameplay.Event.AnimNotify.Attack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Attack_TraceStart, "Gameplay.Event.AnimNotify.Attack.TraceStart");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Attack_TraceEnd, "Gameplay.Event.AnimNotify.Attack.TraceEnd");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Attack_CanActivateNextAttack, "Gameplay.Event.AnimNotify.Attack.CanActivateNextAttack");

	// Rotation Notifies
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Rotation_Lock, "Gameplay.Event.AnimNotify.Rotation.Lock");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Rotation_Unlock, "Gameplay.Event.AnimNotify.Rotation.Unlock");

	// Movement
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Movement_MotionWarping, "Gameplay.Event.AnimNotify.Movement.MotionWarping");

	// Weapon Notifies
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Weapon_StoreLocation, "Gameplay.Event.AnimNotify.Weapon.StoreLocation");

	// Misc Notifies
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Event_AnimNotify_Footstep, "Gameplay.Event.AnimNotify.Footstep");


	// ============================================================
	// ABILITY TRIGGER EVENTS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_TakeDamage, "Gameplay.AbilityTriggerEvent.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_Death_Basic, "Gameplay.AbilityTriggerEvent.Death.Basic");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_Death_Finisher, "Gameplay.AbilityTriggerEvent.Death.Finisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_ComboMelee, "Gameplay.AbilityTriggerEvent.ComboMelee");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_ParryKnockback, "Gameplay.AbilityTriggerEvent.ParryKnockback");


	// ============================================================
	// STATE TAGS
	// ============================================================
	// Base State
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InAir, "Gameplay.State.InAir");

	// Debuff States
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff, "Gameplay.State.Debuff");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff_Stun, "Gameplay.State.Debuff.Stun");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff_Slow, "Gameplay.State.Debuff.Slow");

	// Action Phase States
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Phase, "Gameplay.State.Phase");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Phase_Startup, "Gameplay.State.Phase.Startup");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Phase_Active, "Gameplay.State.Phase.Active");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Phase_Recovery, "Gameplay.State.Phase.Recovery");

	// Movement States
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving, "Gameplay.State.Moving");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Strafing, "Gameplay.State.Moving.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Patrolling, "Gameplay.State.Moving.Patrolling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Walking, "Gameplay.State.Moving.Walking");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Jogging, "Gameplay.State.Moving.Jogging");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Running, "Gameplay.State.Moving.Running");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Dash, "Gameplay.State.Moving.Dash");

	// Combat States
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat, "Gameplay.State.InCombat");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeAttack, "Gameplay.State.InCombat.MeleeAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeCombo, "Gameplay.State.InCombat.MeleeCombo");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeCombo1, "Gameplay.State.InCombat.MeleeCombo1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeCombo2, "Gameplay.State.InCombat.MeleeCombo2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeCombo3, "Gameplay.State.InCombat.MeleeCombo3");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_AirKick, "Gameplay.State.InCombat.AirKick");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_UnparryableAttack, "Gameplay.State.InCombat.UnparryableAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_UnDodgebleAttack, "Gameplay.State.InCombat.UnDodgebleAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_UnstoppableAttack, "Gameplay.State.InCombat.UnstoppableAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_CanInterruptUnstoppableAttack, "Gameplay.State.InCombat.CanInterruptUnstoppableAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_TakeDamage, "Gameplay.State.InCombat.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Parry, "Gameplay.State.InCombat.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_ParryKnockback, "Gameplay.State.InCombat.ParryKnockback");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Vulnerable, "Gameplay.State.InCombat.Vulnerable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_CanActivateFinisher, "Gameplay.State.InCombat.CanActivateFinisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Finisher, "Gameplay.State.InCombat.Finisher");

	// Death States
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Dead, "Gameplay.State.InCombat.Dead");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Dead_Basic, "Gameplay.State.InCombat.Dead.Basic");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Dead_Finisher, "Gameplay.State.InCombat.Dead.Finisher");

	// Targeting States
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_AbilityTargeting, "Gameplay.State.AbilityTargeting");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_AbilityTargeting_Shadow, "Gameplay.State.AbilityTargeting.Shadow");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked, "Gameplay.State.TargetLockSystem.Hero.TargetLocked");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_TargetLockSystem_Enemy_Targeted, "Gameplay.State.TargetLockSystem.Enemy.Targeted");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Rotation_LockTowardsTarget, "Gameplay.State.Rotation.LockTowardsTarget");

	// ============================================================
    // WINDOW TAGS
    // ============================================================
	// Perfect Timing Windows
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Window, "Gameplay.Window");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Window_Perfect, "Gameplay.Window.Perfect");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Window_Perfect_Dodge, "Gameplay.Window.Perfect.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Window_Perfect_Parry, "Gameplay.Window.Perfect.Parry");


	// ============================================================
	// ABILITY TAGS
	// ============================================================
	// Base Combat Abilities
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat, "Gameplay.Ability.Combat");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_TakeDamage, "Gameplay.Ability.Combat.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Parry, "Gameplay.Ability.Combat.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Knocback, "Gameplay.Ability.Combat.Knocback");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack, "Gameplay.Ability.Combat.Attack");

	// Shadow Attacks
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Shadow, "Gameplay.Ability.Combat.Attack.Shadow");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Shadow_Cooldown, "Gameplay.Ability.Combat.Attack.Shadow.Cooldown");

	// Melee Combo Attacks
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_MeleeCombo, "Gameplay.Ability.Combat.Attack.MeleeCombo");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_MeleeCombo_ShadowLinked, "Gameplay.Ability.Combat.Attack.MeleeCombo.ShadowLinked");

	// Melee Finishers
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher, "Gameplay.Ability.Combat.Attack.MeleeFinisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_1, "Gameplay.Ability.Combat.Attack.MeleeFinisher.1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_2, "Gameplay.Ability.Combat.Attack.MeleeFinisher.2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_3, "Gameplay.Ability.Combat.Attack.MeleeFinisher.3");

	// Skill Attacks
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Skill, "Gameplay.Ability.Combat.Attack.Skill");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Skill_BladeStep, "Gameplay.Ability.Combat.Attack.Skill.BladeStep");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Skill_BladeStep_Cooldown, "Gameplay.Ability.Combat.Attack.Skill.BladeStep.Cooldown");

	// Attack Direction Tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Direction, "Gameplay.Ability.Combat.Attack.Direction");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Direction_RightToLeft, "Gameplay.Ability.Combat.Attack.Direction.RightToLeft");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Direction_LeftToRight, "Gameplay.Ability.Combat.Attack.Direction.LeftToRight");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Direction_BottomToTop, "Gameplay.Ability.Combat.Attack.Direction.BottomToTop");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Direction_TopToBottom, "Gameplay.Ability.Combat.Attack.Direction.TopToBottom");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Direction_Forward, "Gameplay.Ability.Combat.Attack.Direction.Forward");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Direction_Backward, "Gameplay.Ability.Combat.Attack.Direction.Backward");

	// Attack Type Tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type, "Gameplay.Ability.Combat.Attack.Type");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type_Light, "Gameplay.Ability.Combat.Attack.Type.Light");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type_Mid, "Gameplay.Ability.Combat.Attack.Type.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type_Heavy, "Gameplay.Ability.Combat.Attack.Type.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type_Unparryable, "Gameplay.Ability.Combat.Attack.Type.Unparryable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type_Unstoppable, "Gameplay.Ability.Combat.Attack.Type.Unstoppable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable, "Gameplay.Ability.Combat.Attack.Type.Undodgeable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Combat_Attack_Type_CanInterruptUnstoppable, "Gameplay.Ability.Combat.Attack.Type.CanInterruptUnstoppable");

	// Movement Abilities
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement, "Gameplay.Ability.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Strafing, "Gameplay.Ability.Movement.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Dash, "Gameplay.Ability.Movement.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Dash_Cooldown, "Gameplay.Ability.Movement.Dash.Cooldown");


	// ============================================================
	// GAMEPLAY EFFECT DATA
	// ============================================================
	// SetByCaller Tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_DamageAmount, "Gameplay.EffectData.SetByCaller.DamageAmount");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_GainHealthAmount, "Gameplay.EffectData.SetByCaller.GainHealthAmount");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_GainManaAmount, "Gameplay.EffectData.SetByCaller.GainManaAmount");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_PostureRegenAmount, "Gameplay.EffectData.SetByCaller.PostureRegenAmount");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_PostureDecreaseAmount, "Gameplay.EffectData.SetByCaller.PostureDecreaseAmount");

	// Effect Modifiers
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_EnableLifeSteal, "Gameplay.EffectData.EnableLifeSteal");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_EnableCriticalDamage, "Gameplay.EffectData.EnableCriticalDamage");


	// ============================================================
	// ENTITY TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Character_Hero, "Gameplay.Entity.Character.Hero");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Character_Enemy, "Gameplay.Entity.Character.Enemy");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Character_Enemy_Training, "Gameplay.Entity.Character.Enemy.Training");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Character_Enemy_Crowd, "Gameplay.Entity.Character.Enemy.Crowd");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Character_Enemy_Boss, "Gameplay.Entity.Character.Enemy.Boss");


	// ============================================================
	// HERO ABILITIES
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility0, "Gameplay.Hero.Ability0");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility1, "Gameplay.Hero.Ability1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility2, "Gameplay.Hero.Ability2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility3, "Gameplay.Hero.Ability3");


	// ============================================================
	// DIRECTION TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction, "Gameplay.Direction");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_Forward, "Gameplay.Direction.Forward");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_Backward, "Gameplay.Direction.Backward");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_Left, "Gameplay.Direction.Left");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_Right, "Gameplay.Direction.Right");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_ForwardLeft, "Gameplay.Direction.ForwardLeft");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_ForwardRight, "Gameplay.Direction.ForwardRight");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_BackwardLeft, "Gameplay.Direction.BackwardLeft");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Direction_BackwardRight, "Gameplay.Direction.BackwardRight");


	// ============================================================
	// TUTORIAL TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Tutorial_Dash, "Gameplay.Tutorial.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Tutorial_MeleeCombo, "Gameplay.Tutorial.MeleeCombo");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Tutorial_TargetLock, "Gameplay.Tutorial.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Tutorial_ShadowAttack, "Gameplay.Tutorial.ShadowAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Tutorial_ShadowFinisher, "Gameplay.Tutorial.ShadowFinisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Tutorial_Parry, "Gameplay.Tutorial.Parry");



	/**********************************************
	*----------------<Filter Tags>----------------*
	* Add all of the Filter related tags to here. *
	***********************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_Filter, "Filter");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Filter_Team, "Filter.Team");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Filter_Team_Friend, "Filter.Team.Friend");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Filter_Team_Hostile, "Filter.Team.Hostile");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Filter_Team_Neutral, "Filter.Team.Neutral");



	/***************************************************
	*----------------<GameplayCue Tags>----------------*
	* Add all of the GameplayCue related tags to here. *
	****************************************************/

	// ============================================================
	// ATTACHED EFFECT TEXT CUES
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_PhysicalDamage, "GameplayCue.AttachedEffectText.PhysicalDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_CriticalDamage, "GameplayCue.AttachedEffectText.CriticalDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_Bleed, "GameplayCue.AttachedEffectText.Bleed");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_GainHealth, "GameplayCue.AttachedEffectText.GainHealth");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_GainMana, "GameplayCue.AttachedEffectText.GainMana");


	// ============================================================
	// HERO MESSAGE HUD CUES
	// ============================================================
	// Status Messages
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_LevelUp, "GameplayCue.HeroMessageHUD.LevelUp");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Died, "GameplayCue.HeroMessageHUD.Died");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Stunned, "GameplayCue.HeroMessageHUD.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Slowed, "GameplayCue.HeroMessageHUD.Slowed");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_GainPhysicalArmor, "GameplayCue.HeroMessageHUD.GainPhysicalArmor");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_GainLifeSteal, "GameplayCue.HeroMessageHUD.GainLifeSteal");

	// Tutorial Messages
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Tutorial_QuestCompleted, "GameplayCue.HeroMessageHUD.Tutorial.QuestCompleted");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Tutorial_TutorialCompleted, "GameplayCue.HeroMessageHUD.Tutorial.TutorialCompleted");


	// ============================================================
	// VISUAL EFFECTS (VFX) CUES
	// ============================================================
	// Blood Effects - Slash
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood, "GameplayCue.VFX.Blood");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Slash, "GameplayCue.VFX.Blood.Slash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Slash_Low, "GameplayCue.VFX.Blood.Slash.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Slash_Hero_Low, "GameplayCue.VFX.Blood.Slash.Hero.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Slash_Mid, "GameplayCue.VFX.Blood.Slash.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Slash_Hero_Mid, "GameplayCue.VFX.Blood.Slash.Hero.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Slash_High, "GameplayCue.VFX.Blood.Slash.High");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Slash_Hero_High, "GameplayCue.VFX.Blood.Slash.Hero.High");

	// Blood Effects - Splash
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splash, "GameplayCue.VFX.Blood.Splash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splash_Low, "GameplayCue.VFX.Blood.Splash.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splash_Hero_Low, "GameplayCue.VFX.Blood.Splash.Hero.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splash_Mid, "GameplayCue.VFX.Blood.Splash.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splash_Hero_Mid, "GameplayCue.VFX.Blood.Splash.Hero.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splash_High, "GameplayCue.VFX.Blood.Splash.High");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splash_Hero_High, "GameplayCue.VFX.Blood.Splash.Hero.High");

	// Blood Effects - Splatter
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splatter, "GameplayCue.VFX.Blood.Splatter");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splatter_Low, "GameplayCue.VFX.Blood.Splatter.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splatter_Hero_Low, "GameplayCue.VFX.Blood.Splatter.Hero.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splatter_Mid, "GameplayCue.VFX.Blood.Splatter.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splatter_Hero_Mid, "GameplayCue.VFX.Blood.Splatter.Hero.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splatter_High, "GameplayCue.VFX.Blood.Splatter.High");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Splatter_Hero_High, "GameplayCue.VFX.Blood.Splatter.Hero.High");

	// Blood Effects - Stab
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Stab, "GameplayCue.VFX.Blood.Stab");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Stab_Low, "GameplayCue.VFX.Blood.Stab.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Stab_Hero_Low, "GameplayCue.VFX.Blood.Stab.Hero.Low");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Stab_Mid, "GameplayCue.VFX.Blood.Stab.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Stab_Hero_Mid, "GameplayCue.VFX.Blood.Stab.Hero.Mid");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Stab_High, "GameplayCue.VFX.Blood.Stab.High");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood_Stab_Hero_High, "GameplayCue.VFX.Blood.Stab.Hero.High");

	// Special VFX
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Parry, "GameplayCue.VFX.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Heal, "GameplayCue.VFX.Heal");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Hero_Ghost, "GameplayCue.VFX.Hero.Ghost");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Boss_Ghost, "GameplayCue.VFX.Boss.Ghost");

	// Dissolve VFX
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Dissolve_CharacterDead, "GameplayCue.VFX.Dissolve.CharacterDead");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Dissolve_SpawnShadow_Hero, "GameplayCue.VFX.Dissolve.SpawnShadow.Hero");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Dissolve_SpawnShadow_Boss, "GameplayCue.VFX.Dissolve.SpawnShadow.Boss");


	// ============================================================
	// SOUND CUES
	// ============================================================
	// Play On Target
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnTarget, "GameplayCue.Sound.PlayOnTarget");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnTarget_TakeDamage, "GameplayCue.Sound.PlayOnTarget.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnTarget_Finisher, "GameplayCue.Sound.PlayOnTarget.Finisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnTarget_Kick, "GameplayCue.Sound.PlayOnTarget.Kick");

	// Play On Source
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnSource, "GameplayCue.Sound.PlayOnSource");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnSource_Footstep, "GameplayCue.Sound.PlayOnSource.Footstep");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnSource_ParryKnockback, "GameplayCue.Sound.PlayOnSource.ParryKnockback");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnSource_AttackSwing, "GameplayCue.Sound.PlayOnSource.AttackSwing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnSource_Death, "GameplayCue.Sound.PlayOnSource.Death");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnSource_LevelUp, "GameplayCue.Sound.PlayOnSource.LevelUp");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_PlayOnSource_Kick, "GameplayCue.Sound.PlayOnSource.Kick");


	// ============================================================
	// COMBAT CAMERA & EFFECTS CUES
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_Camera_Shake, "GameplayCue.Combat.Camera.Shake");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_Camera_ZoomIn, "GameplayCue.Combat.Camera.ZoomIn");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_Camera_ZoomOut, "GameplayCue.Combat.Camera.ZoomOut");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_HitPasue, "GameplayCue.Combat.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_HologramSlowMo, "GameplayCue.Combat.HologramSlowMo");



	/************************************************
	*-------------------<AI Tags>-------------------*
	* Add all of the AI related tags to here.       *
	*************************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI, "AI");

	// ============================================================
    // AI STATE EVENTS
    // ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateEvent, "AI.StateEvent");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateEvent_VulnerableTagAdded, "AI.StateEvent.VulnerableTagAdded");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateEvent_InComingAttack, "AI.StateEvent.InComingAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateEvent_TakeHit, "AI.StateEvent.TakeHit");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateEvent_TargetDetected, "AI.StateEvent.TargetDetected");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateEvent_BackupReaction, "AI.StateEvent.BackupReaction");


	// ============================================================
	// AI STATE TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State, "AI.State");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Patrolling, "AI.State.Patrolling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Movement, "AI.State.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Attack, "AI.State.Attack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_InComingAttack, "AI.State.InComingAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_TakeHit, "AI.State.TakeHit");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Vulnerable, "AI.State.Vulnerable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_BackupReaction, "AI.State.BackupReaction");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_HeroFinisher, "AI.State.HeroFinisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_IsAttackIntender, "AI.State.IsAttackIntender");


	// ============================================================
	// AI ABILITY TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_PhantomDash, "AI.Ability.PhantomDash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_PhantomDash_Cooldown, "AI.Ability.PhantomDash.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_TwinSlashUp, "AI.Ability.TwinSlashUp");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_TwinSlashUp_Cooldown, "AI.Ability.TwinSlashUp.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_LineBreaker, "AI.Ability.LineBreaker");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_LineBreaker_Cooldown, "AI.Ability.LineBreaker.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_ShadowAttack, "AI.Ability.ShadowAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_ShadowAttack_Cooldown, "AI.Ability.ShadowAttack.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_Dash, "AI.Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_Dash_Cooldown, "AI.Ability.Dash.Cooldown");


	// ============================================================
	// AI ABILITY TRIGGER EVENTS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement, "AI.AbilityTriggerEvent.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_Patrolling, "AI.AbilityTriggerEvent.Movement.Patrolling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_Strafing_Orbit, "AI.AbilityTriggerEvent.Movement.Strafing.Orbit");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_Strafing_AttackIntend, "AI.AbilityTriggerEvent.Movement.Strafing.AttackIntend");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_ChaseTarget, "AI.AbilityTriggerEvent.Movement.ChaseTarget");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_Dash, "AI.AbilityTriggerEvent.Movement.Dash");


	// ============================================================
	// AI DIRECTION TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction, "AI.Direction");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Resolved, "AI.Direction.Resolved");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Resolved_Forward, "AI.Direction.Resolved.Forward");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Resolved_Backward, "AI.Direction.Resolved.Backward");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Resolved_Left, "AI.Direction.Resolved.Left");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Resolved_Right, "AI.Direction.Resolved.Right");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Policy, "AI.Direction.Policy");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Policy_Random, "AI.Direction.Policy.Random");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Direction_Policy_PlayerLastDirection, "AI.Direction.Policy.LastPlayerDirection");



	/************************************************
	*-------------------<UI Tags>-------------------*
	* Add all of the UI related tags to here.       *
	*************************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_UI, "UI");


	// ============================================================
	// UI HIT TYPE TEXT TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Critical, "UI.HitTypeText.Critical");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Blind, "UI.HitTypeText.Blind");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Physical, "UI.HitTypeText.Physical");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Magical, "UI.HitTypeText.Magical");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Healing, "UI.HitTypeText.Heal");


	// ============================================================
	// UI COOLDOWN TAGS
	// ============================================================
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability0, "UI.Cooldown.Ability0");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability1, "UI.Cooldown.Ability1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability2, "UI.Cooldown.Ability2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability3, "UI.Cooldown.Ability3");
}