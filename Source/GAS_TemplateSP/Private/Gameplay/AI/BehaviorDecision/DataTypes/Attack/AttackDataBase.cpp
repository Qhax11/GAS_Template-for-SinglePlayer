// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"

bool UAttackDataBase::IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug) const
{
	// Ability tanýmlý deðilse bu attack seçilemez
	if (!AbilityClass)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EAttackDisableReason::InvalidAbility;
		}
		return false;
	}

	// Target yoksa saldýrý olmaz
	if (!Context.Target)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EAttackDisableReason::InvalidTarget;
		}
		return false;
	}

	if (OutDebug)
	{
		OutDebug->Reason = EAttackDisableReason::None;
	}

	return true;
}

bool UAttackDataBase::PassesChance(const FAttackDecisionContext& Context, FAttackChanceDebug* OutDebug) const
{
	// Base attack'lerde chance uygulanmaz
    // Override edilirse özel saldýrýlar riskli yapýlabilir

	if (OutDebug)
	{
		OutDebug->Roll = 0.f;
		OutDebug->Threshold = 1.f;
	}

	return true;
}

float UAttackDataBase::GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug) const
{
	float BehaviorScore = 0.f;
	float ComboScore = 0.f;

	// Behavior state modifier
	if (const float* Modifier = BehaviorStateScoreModifiers.Find(Context.BehaviorState))
	{
		BehaviorScore = *Modifier;
	}

	// Combo bonus
	if (bIsComboAttack && Context.bIsInCombo)
	{
		// Base combo mantýðý: zincirdeysen küçük bir teþvik
		ComboScore = 0.25f;
	}

	const float TotalScore = BehaviorScore + ComboScore + ScoreBias;

	if (OutDebug)
	{
		OutDebug->BehaviorScore = BehaviorScore;
		OutDebug->ComboScore = ComboScore;
		OutDebug->Bias = ScoreBias;
		OutDebug->Total = TotalScore;
	}

	return TotalScore;
}
