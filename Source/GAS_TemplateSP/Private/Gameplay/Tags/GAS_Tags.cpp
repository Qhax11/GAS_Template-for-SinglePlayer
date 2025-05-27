// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Tags/GAS_Tags.h"

namespace GAS_Tags
{
	/************************************************
	*----------------<Gameplay Tags>----------------*
	* Add all of the Gameplay related tags to here. *
	*************************************************/

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay, "Gameplay");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Health, "Gameplay.Attribute.Health");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Health_Full, "Gameplay.Attribute.Health.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Health_Regen, "Gameplay.Attribute.Health.Regen");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Mana, "Gameplay.Attribute.Mana");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Mana_Full, "Gameplay.Attribute.Mana.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Mana_Regen, "Gameplay.Attribute.Mana.Regen");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Posture, "Gameplay.Attribute.Posture");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Posture_Full, "Gameplay.Attribute.Posture.Full");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Attribute_Posture_Regen, "Gameplay.Attribute.Posture.Regen");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent, "Gameplay.AttackEvent");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent_TraceStart, "Gameplay.AttackEvent.TraceStart");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent_TraceEnd, "Gameplay.AttackEvent.TraceEnd");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AttackEvent_CanActivateNextAttack, "Gameplay.AttackEvent.CanActivateNextAttack");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmune, "Gameplay.DamageImmune");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_TakeDamage, "Gameplay.AbilityTriggerEvent.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_Death, "Gameplay.AbilityTriggerEvent.Death");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_FinisherDeath, "Gameplay.AbilityTriggerEvent.FinisherDeath");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_ComboMelee, "Gameplay.AbilityTriggerEvent.ComboMelee");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityTriggerEvent_ParryKnockback, "Gameplay.AbilityTriggerEvent.ParryKnockback");
	
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff, "Gameplay.State.Debuff");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff_Stun, "Gameplay.State.Debuff.Stun");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Debuff_Slow, "Gameplay.State.Debuff.Slow");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Strafing, "Gameplay.State.Moving.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Patrolling, "Gameplay.State.Moving.Patrolling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Walking, "Gameplay.State.Moving.Walking");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Jogging, "Gameplay.State.Moving.Jogging");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Running, "Gameplay.State.Moving.Running");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Moving_Dash, "Gameplay.State.Moving.Dash");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Dead, "Gameplay.State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_FinisherDead, "Gameplay.State.FinisherDead");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_Vulnerable, "Gameplay.State.Vulnerable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_CanActivateFinisher, "Gameplay.State.CanActivateFinisher");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat, "Gameplay.State.InCombat");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_TakeDamage, "Gameplay.State.InCombat.TakeDamage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Parry, "Gameplay.State.InCombat.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_ParryKnockback, "Gameplay.State.InCombat.ParryKnockback");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_State_InCombat_Finisher, "Gameplay.State.InCombat.Finisher");
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
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_SetByCaller_PostureRegenAmount, "Gameplay.EffectData.SetByCaller.PostureRegenAmount");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_EnableLifeSteal, "Gameplay.EffectData.EnableLifeSteal");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_EffectData_EnableCriticalDamage, "Gameplay.EffectData.EnableCriticalDamage");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Hero, "Gameplay.Entity.Hero");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Enemy, "Gameplay.Entity.Enemy");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Entity_Boss, "Gameplay.Entity.Boss");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility0, "Gameplay.Hero.Ability0");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility1, "Gameplay.Hero.Ability1");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility2, "Gameplay.Hero.Ability2");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_HeroAbility3, "Gameplay.Hero.Ability3");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Parry, "Gameplay.Ability.Parry");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Attack, "Gameplay.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Attack_Shadow, "Gameplay.Ability.Attack.Shadow");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Attack_Shadow_Cooldown, "Gameplay.Ability.Attack.Shadow.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Attack_MeleeCombo, "Gameplay.Ability.Attack.MeleeCombo");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Attack_MeleeCombo_ShadowLinked, "Gameplay.Ability.Attack.MeleeCombo.ShadowLinked");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement, "Gameplay.Ability.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Strafing, "Gameplay.Ability.Movement.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Dash, "Gameplay.Ability.Movement.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Ability_Movement_Dash_Cooldown, "Gameplay.Ability.Movement.Dash.Cooldown");

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

	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Tutorial_QuestCompleted, "GameplayCue.HeroMessageHUD.Tutorial.QuestCompleted");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HeroMessageHUD_Tutorial_TutorialCompleted, "GameplayCue.HeroMessageHUD.Tutorial.TutorialCompleted");

	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Blood, "GameplayCue.VFX.Blood");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Parry, "GameplayCue.VFX.Parry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Heal, "GameplayCue.VFX.Heal");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Hero_Ghost, "GameplayCue.VFX.Hero.Ghost");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Boss_Ghost, "GameplayCue.VFX.Boss.Ghost");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Dissolve_CharacterDead, "GameplayCue.VFX.Dissolve.CharacterDead");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Dissolve_SpawnShadow_Hero, "GameplayCue.VFX.Dissolve.SpawnShadow.Hero");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_VFX_Dissolve_SpawnShadow_Boss, "GameplayCue.VFX.Dissolve.SpawnShadow.Boss");
	
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

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State, "AI.State");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Patrolling, "AI.State.Patrolling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Movement, "AI.State.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Attack, "AI.State.Attack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_InComingAttack, "AI.State.InComingAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_Vulnerable, "AI.State.Vulnerable");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_DetectedPlayer, "AI.StateTreeEvent.DetectedPlayer");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_PlayerStartedAttack, "AI.StateTreeEvent.PlayerStartedAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_State_Vulnerable, "AI.StateTreeEvent.State.Vulnerable");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_ExecuteShadowAttack, "AI.StateTreeEvent.ExecuteShadowAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTreeEvent_Crowd_HeroFinisher, "AI.StateTreeEvent.Crowd.HeroFinisher");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_StateTree_State_ComingAttack, "AI.StateTree.State.ComingAttack");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_State_IsAttackIntender, "AI.State.IsAttackIntender");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_PhantomDash, "AI.Ability.PhantomDash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_PhantomDash_Cooldown, "AI.Ability.PhantomDash.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_LineBreaker, "AI.Ability.LineBreaker");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_LineBreaker_Cooldown, "AI.Ability.LineBreaker.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_ShadowAttack, "AI.Ability.ShadowAttack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_ShadowAttack_Cooldown, "AI.Ability.ShadowAttack.Cooldown");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_Dash, "AI.Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_Ability_Dash_Cooldown, "AI.Ability.Dash.Cooldown");

	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement, "AI.AbilityTriggerEvent.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_Patrolling, "AI.AbilityTriggerEvent.Movement.Patrolling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_Strafing, "AI.AbilityTriggerEvent.Movement.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_ChaseTarget, "AI.AbilityTriggerEvent.Movement.ChaseTarget");
	UE_DEFINE_GAMEPLAY_TAG(TAG_AI_AbilityTriggerEvent_Movement_Dash, "AI.AbilityTriggerEvent.Movement.Dash");

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