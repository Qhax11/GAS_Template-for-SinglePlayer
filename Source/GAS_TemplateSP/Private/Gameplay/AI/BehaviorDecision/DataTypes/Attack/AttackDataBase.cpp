// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

bool UAttackDataBase::IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug) const
{
	// Target yoksa saldýrý olmaz
	if (!Context.Target)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EAttackDisableReason::InvalidContext;
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
	float IntentScore = GetIntentScore(Context);
	float DistanceScore = GetDistanceScore(Context);

	const float TotalScore = IntentScore + DistanceScore + ScoreBias;

	if (OutDebug)
	{
		OutDebug->CurrentIntent = Context.EnemyIntent;
		OutDebug->IntentScore = IntentScore;
		OutDebug->DistanceScore = DistanceScore;
		OutDebug->Bias = ScoreBias;
		OutDebug->Total = TotalScore;
	}

	return TotalScore;
}

float UAttackDataBase::GetIntentScore(const FAttackDecisionContext& Context) const
{
	float Score = 0.0f;

	if (const float* FoundScore = IntentScoreModifiers.Find(Context.EnemyIntent))
	{
		Score += *FoundScore;
	}

	return Score;
}

float UAttackDataBase::GetDistanceScore(const FAttackDecisionContext& Context) const
{
	if (!IsValid(Context.Owner) || !IsValid(Context.Target))
	{
		return 0.f;
	}

	const float MinRange = GetMinRange();
	const float MaxRange = GetMaxRange();
	const float Distance = CombatDistance::GetDistance(Context.Owner, Context.Target);

	// Çok yakýn ? aðýr ceza
	if (Distance < MinRange)
	{
		return -1.0f;
	}

	// Çok uzak ? aðýr ceza
	if (Distance > MaxRange)
	{
		return -0.5f;
	}

	// Min–Max arasý: ideal noktaya göre normalize
	const float IdealDistance = MaxRange;
	const float RangeSpan = MaxRange - MinRange;

	if (RangeSpan <= 0.f)
	{
		return 0.f;
	}

	const float DistFromIdeal = FMath::Abs(Distance - IdealDistance);
	const float Normalized = 1.f - (DistFromIdeal / RangeSpan);

	return FMath::Lerp(-1.f, 1.f, Normalized);
}


