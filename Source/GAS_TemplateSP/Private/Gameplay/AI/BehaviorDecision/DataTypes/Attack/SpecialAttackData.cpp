// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/SpecialAttackData.h"

USpecialAttackData::USpecialAttackData()
{
	AttackMovementTag = GAS_Tags::TAG_AI_Decision_Attack_Movement_HighPressure_Walk;
}

bool USpecialAttackData::IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug) const
{
	// If its fail in parent, fail here too
	if(!Super::IsEnable(Context, OutDebug))
	{
		return false;
	}

	// Ability tanýmlý deðilse bu attack seçilemez
	if (!AbilityClass)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EAttackDisableReason::InvalidAbility;
		}
		return false;
	}

	// Ability cooldown'daysa bu attack seçilemez
	if (Context.OwnerASC->HasMatchingGameplayTag(AbilityCooldownTag))
	{
		if (OutDebug)
		{
			OutDebug->Reason = EAttackDisableReason::OnCooldown;
		}
		return false;
	}

	return true;
}

bool USpecialAttackData::PassesChance(const FAttackDecisionContext& Context, FAttackChanceDebug* OutDebug) const
{
	// If its fail in parent, fail here too
	if (!Super::PassesChance(Context, OutDebug))
	{
		return false;
	}

	return false;
}

float USpecialAttackData::GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug) const
{
	float ParentScore = Super::GetScore(Context, OutDebug);
	return ParentScore;
}

float USpecialAttackData::GetMinRange() const
{
	if (!AbilityClass)
	{
		return 0.f;
	}

	return AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->MinRange;
}

float USpecialAttackData::GetMaxRange() const
{
	if (!AbilityClass)
	{
		return 0.f;
	}

	return AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->MaxRange;
}

EAttackExecutionType USpecialAttackData::GetExecutionType() const
{
	return EAttackExecutionType::Single;
}

