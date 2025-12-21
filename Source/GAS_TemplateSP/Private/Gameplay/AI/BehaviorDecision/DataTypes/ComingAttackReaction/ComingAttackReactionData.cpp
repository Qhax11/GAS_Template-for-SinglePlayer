// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

bool UComingAttackReactionData::IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug) const
{
	// Too late to react
	if (ComingAttackPayload.ComingAttackHitTime < MinimumTimeBeforeHitToReact)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::TooLate;
		}
		return false;
	}

	if (!IsAttackInRange(ComingAttackPayload))
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::OutOfRange;
		}
		return false;
	}

	if (OutDebug)
	{
		OutDebug->Reason = EReactionDisableReason::None;
	}

	return true;
}

bool UComingAttackReactionData::PassesChanceRoll(const UAbilitySystemComponent* ASC, FReactionChanceDebug* OutDebug) const
{
	const float Roll = FMath::FRandRange(0.f, 1.f);
	const bool bPassed = Roll <= BaseChance;

	if (OutDebug)
	{
		OutDebug->Roll = Roll;
		OutDebug->Threshold = BaseChance;
		OutDebug->Reason = bPassed
			? EReactionChanceFailReason::None
			: EReactionChanceFailReason::RandomRollFailed;
	}

	return bPassed;
}

float UComingAttackReactionData::GetScore(const FComingAttackPayload& ComingAttackPayload, EEnemyIntent EnemyIntent, FReactionScoreDebug* OutDebug) const
{
	float IntentScore = GetIntentScore(ComingAttackPayload, EnemyIntent);
	float TagScore = CalculateTagScore(ComingAttackPayload);

	float Total = IntentScore + TagScore + ScoreBias;

	if (OutDebug) 
	{
		OutDebug->IntentScore = IntentScore;
		OutDebug->TagScore = TagScore;
		OutDebug->Bias = ScoreBias;
		OutDebug->Total = Total;
	}

	return Total;
}

bool UComingAttackReactionData::IsAttackInRange(const FComingAttackPayload& ComingAttackPayload) const
{
	return CombatDistance::IsInRange(ComingAttackPayload.Attacker, ComingAttackPayload.Defender, ComingAttackPayload.ComingAttack->MaxRange);
}

float UComingAttackReactionData::GetIntentScore(const FComingAttackPayload& ComingAttackPayload, EEnemyIntent EnemyIntent) const
{
	float Score = 0.0f;

	if (const float* FoundScore = EnemyIntentScoreModifiers.Find(EnemyIntent))
	{
		Score += *FoundScore;
	}

	return Score;
}

float UComingAttackReactionData::CalculateTagScore(const FComingAttackPayload& ComingAttackPayload) const
{
	float Score = 0.f;

	for (const auto& Pair : TagScoreModifiers)
	{
		if (ComingAttackPayload.ComingAttackTags.HasTag(Pair.Key))
		{
			Score += Pair.Value;
		}
	}

	return Score;
}
