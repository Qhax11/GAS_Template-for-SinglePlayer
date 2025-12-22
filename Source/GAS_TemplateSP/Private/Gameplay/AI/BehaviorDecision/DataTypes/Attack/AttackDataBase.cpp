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
	return 0.f;
	/*
	if (!AbilityClass || !IsValid(Context.Owner) || !IsValid(Context.Target))
	{
		return 0.f;
	}

	const float AttackRange = AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->MaxRange;
	if (AttackRange <= 0.f)
	{
		return 0.f;
	}

	const float Distance = CombatDistance::GetDistance(Context.Owner, Context.Target);

	const float Alpha = FMath::Clamp(Distance / AttackRange, 0.f, 1.f);

	// Near = 1.0, Far = 0.0
	return FMath::Lerp(1.f, 0.f, Alpha);
	*/
}

/*
float UAttackDataBase::CalculateScoreBasedOnTargetDistance(FAttackData AttackData, float DistanceToTarget)
{
	float AbilityMinRange = AttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->MinRange;
	float AbilityMaxRange = AttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->MaxRange;
	if (AbilityMaxRange <= 0.f)
	{
		return 0.0f;
	}

	// Saldýrýnýn ideal noktasý: MaxRange
	float DistanceFromIdeal = FMath::Abs(DistanceToTarget - AbilityMaxRange);

	// Skoru mesafeye göre ters orantýlý olarak hesapla
	float Score = 1.f - (DistanceFromIdeal / AbilityMaxRange);

	// Minimum Range'in ALTINDA mesafedeyse ekstra ceza uygula (isteðe baðlý)
	if (DistanceToTarget < AbilityMinRange)
	{
		Score = -100; // Çok yakýnsa etkisizleþtir
	}

	return Score;
}

float UAttackDataBase::CalculateComboScore(FAttackData AttackData)
{
	// If there is no valid last attack or it wasn't part of a combo chain
	if (!LastSelectedAttackAbilityData.AbilityClass || !LastSelectedAttackAbilityData.bIsComboAttack)
	{
		return 0.0f;
	}

	// If the current candidate isn't a combo attack, skip
	if (!AttackData.bIsComboAttack)
	{
		return 0.0f;
	}

	// Expected combo index is always the next step after the last selected
	int32 ExpectedNextIndex = LastSelectedAttackAbilityData.ComboIndex + 1;

	// If this attack matches the expected combo step, give it a strong score
	if (AttackData.ComboIndex == ExpectedNextIndex)
	{
		return 3.0f;
	}

	return 0.0f;
}

*/
