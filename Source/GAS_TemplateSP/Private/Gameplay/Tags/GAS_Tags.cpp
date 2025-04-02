// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Tags/GAS_Tags.h"

namespace GAS_Tags
{
	/************************************************
	*----------------<Gameplay Tags>----------------*
	* Add all of the Gameplay related tags to here. *
	*************************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay, "Gameplay");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Health, "Gameplay.Health");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Health_Full, "Gameplay.Health.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Health_Regen, "Gameplay.Health.Regen");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Mana, "Gameplay.Mana");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Mana_Full, "Gameplay.Mana.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Mana_Regen, "Gameplay.Mana.Regen");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent, "Gameplay.AttackEvent");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent_TraceStart, "Gameplay.AttackEvent.TraceStart");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent_TraceEnd, "Gameplay.AttackEvent.TraceEnd");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent_CanActivateNextAttack, "Gameplay.AttackEvent.CanActivateNextAttack");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmune, "Gameplay.DamageImmune");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_TakeDamage, "Gameplay.AbilityTriggerEvent.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_Death, "Gameplay.AbilityTriggerEvent.Death");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_ComboMelee, "Gameplay.AbilityTriggerEvent.ComboMelee");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_ParryKnockback, "Gameplay.AbilityTriggerEvent.ParryKnockback");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_AI_Movement, "Gameplay.AbilityTriggerEvent.AI.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_AI_Movement_Strafing, "Gameplay.AbilityTriggerEvent.AI.Movement.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_AI_Movement_ChaseTarget, "Gameplay.AbilityTriggerEvent.AI.Movement.ChaseTarget");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_AI_Movement_Dash, "Gameplay.AbilityTriggerEvent.AI.Movement.Dash");
	
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff, "Gameplay.State.Debuff");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff_Stun, "Gameplay.State.Debuff.Stun");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff_Slow, "Gameplay.State.Debuff.Slow");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Strafing, "Gameplay.State.Moving.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Walking, "Gameplay.State.Moving.Walking");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Jogging, "Gameplay.State.Moving.Jogging");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Running, "Gameplay.State.Moving.Running");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Dash, "Gameplay.State.Moving.Dash");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Dead, "Gameplay.State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_DeadWithFinisher, "Gameplay.State.DeadWithFinisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Vulnerable, "Gameplay.State.Vulnerable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_CanActivateFinisher, "Gameplay.State.CanActivateFinisher");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat, "Gameplay.State.InCombat");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Parry, "Gameplay.State.InCombat.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeCombo1, "Gameplay.State.InCombat.MeleeCombo1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeCombo2, "Gameplay.State.InCombat.MeleeCombo2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_MeleeCombo3, "Gameplay.State.InCombat.MeleeCombo3");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_UnstoppableAttack, "Gameplay.State.InCombat.UnstoppableAttack");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_AbilityTargeting, "Gameplay.State.AbilityTargeting");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_AbilityTargeting_Shadow, "Gameplay.State.AbilityTargeting.Shadow");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked, "Gameplay.State.TargetLockSystem.Hero.TargetLocked");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_TargetLockSystem_Enemy_Targeted, "Gameplay.State.TargetLockSystem.Enemy.Targeted");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_DamageAmount, "Gameplay.EffectData.SetByCaller.DamageAmount");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_GainHealthAmount, "Gameplay.EffectData.SetByCaller.GainHealthAmount");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_GainManaAmount, "Gameplay.EffectData.SetByCaller.GainManaAmount");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_EnableLifeSteal, "Gameplay.EffectData.EnableLifeSteal");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_EnableCriticalDamage, "Gameplay.EffectData.EnableCriticalDamage");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Hero, "Gameplay.Entity.Hero");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Enemy, "Gameplay.Entity.Enemy");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Boss, "Gameplay.Entity.Boss");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility0, "Gameplay.Hero.Ability0");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility1, "Gameplay.Hero.Ability1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility2, "Gameplay.Hero.Ability2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility3, "Gameplay.Hero.Ability3");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_MeleeCombo, "Gameplay.Ability.MeleeCombo");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Finisher, "Gameplay.Ability.Finisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Parry, "Gameplay.Ability.Parry");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement, "Gameplay.Ability.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Strafing, "Gameplay.Ability.Movement.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Dash, "Gameplay.Ability.Movement.Dash");

	// ****** UTILITIES ****** //
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Utilities_Direction, "Gameplay.Utilities.Direction");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Utilities_Direction_Forward, "Gameplay.Utilities.Direction.Forward");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Utilities_Direction_Backward, "Gameplay.Utilities.Direction.Backward");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Utilities_Direction_Left, "Gameplay.Utilities.Direction.Left");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Utilities_Direction_Right, "Gameplay.Utilities.Direction.Right");

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

	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_PhysicalDamage, "GameplayCue.AttachedEffectText.PhysicalDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_CriticalDamage, "GameplayCue.AttachedEffectText.CriticalDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_Bleed, "GameplayCue.AttachedEffectText.Bleed");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_GainHealth, "GameplayCue.AttachedEffectText.GainHealth");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_AttachedEffectText_GainMana, "GameplayCue.AttachedEffectText.GainMana");

	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_LevelUp, "GameplayCue.HeroMessageHUD.LevelUp");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Died, "GameplayCue.HeroMessageHUD.Died");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Stunned, "GameplayCue.HeroMessageHUD.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Slowed, "GameplayCue.HeroMessageHUD.Slowed");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_GainPhysicalArmor, "GameplayCue.HeroMessageHUD.GainPhysicalArmor");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_GainLifeSteal, "GameplayCue.HeroMessageHUD.GainLifeSteal");

	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood, "GameplayCue.VFX.Blood");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Parry, "GameplayCue.VFX.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Heal, "GameplayCue.VFX.Heal");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Ghost, "GameplayCue.VFX.Ghost");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Dissolve_CharacterDead, "GameplayCue.VFX.Dissolve.CharacterDead");
	
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound, "GameplayCue.Sound");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_TakeDamage, "GameplayCue.Sound.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_Parry, "GameplayCue.Sound.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_AttackSwing, "GameplayCue.Sound.AttackSwing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_Death, "GameplayCue.Sound.Death");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Sound_LevelUp, "GameplayCue.Sound.LevelUp");

	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_HitPasue, "GameplayCue.Combat.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_CameraShake, "GameplayCue.Combat.CameraShake");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Combat_HologramSlowMo, "GameplayCue.Combat.HologramSlowMo");

	/************************************************
	*-------------------<AI Tags>-------------------*
	* Add all of the AI related tags to here.       *
	*************************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI, "AI");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_DetectedPlayer, "AI.StateTreeEvent.DetectedPlayer");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_PlayerStartedAttack, "AI.StateTreeEvent.PlayerStartedAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_State_DeadWithFinisher, "AI.StateTreeEvent.State_DeadWithFinisher");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_State_Vulnerable, "AI.StateTreeEvent.State.Vulnerable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_State_Strafing, "AI.StateTreeEvent.State.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_State_Engage, "AI.StateTreeEvent.State.Engage");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_ExecuteSelectedAttack, "AI.StateTreeEvent.ExecuteSelectedAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_ExecuteShadowAttack, "AI.StateTreeEvent.ExecuteShadowAttack");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Attack, "AI.State.Attack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_MovingToAttack, "AI.State.MovingToAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_SoCloseToHero, "AI.State.SoCloseToHero");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_PhantomDash, "AI.Ability.PhantomDash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_PhantomDash_Cooldown, "AI.Ability.PhantomDash.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_ShadowAttack, "AI.Ability.ShadowAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_ShadowAttack_Cooldown, "AI.Ability.ShadowAttack.Cooldown");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_Dash, "AI.Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_Dash_Cooldown, "AI.Ability.Dash.Cooldown");

	/************************************************
	*-------------------<UI Tags>-------------------*
	* Add all of the UI related tags to here.       *
	*************************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_UI, "UI");

	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Critical, "UI.HitTypeText.Critical");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Blind, "UI.HitTypeText.Blind");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Physical, "UI.HitTypeText.Physical");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Magical, "UI.HitTypeText.Magical");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_HitTypeText_Healing, "UI.HitTypeText.Heal");

	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability0, "UI.Cooldown.Ability0");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability1, "UI.Cooldown.Ability1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability2, "UI.Cooldown.Ability2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Cooldown_Ability3, "UI.Cooldown.Ability3");
}