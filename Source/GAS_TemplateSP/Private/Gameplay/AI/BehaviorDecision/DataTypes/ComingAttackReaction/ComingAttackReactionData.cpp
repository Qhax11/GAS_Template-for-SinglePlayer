// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"


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

	if (!CheckDistance(ComingAttackPayload))
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::InvalidDistance;
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

float UComingAttackReactionData::GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState, FReactionScoreDebug* OutDebug) const
{
	float BehaviorStateScore = CalculateBehaviorStateScore(ComingAttackPayload, BehaviorState);
	float TagScore = CalculateTagScore(ComingAttackPayload);
	float Total = BehaviorStateScore + TagScore + ScoreBias;

	if (OutDebug) 
	{
		OutDebug->BehaviorStateScore = BehaviorStateScore;
		OutDebug->TagScore = TagScore;
		OutDebug->Bias = ScoreBias;
		OutDebug->Total = Total;
	}

	return Total;
}

float UComingAttackReactionData::CalculateBehaviorStateScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState) const
{
	if (const float* Mod = BehaviorStateScoreModifiers.Find(BehaviorState))
	{
		return *Mod;
	}

	return 0.f;
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

bool UComingAttackReactionData::CheckDistance(const FComingAttackPayload& ComingAttackPayload) const
{
	if (!ComingAttackPayload.ComingAttack)
	{
		return false;
	}

	AActor* Attacker = ComingAttackPayload.Attacker;
	AActor* Defender = ComingAttackPayload.Defender;

	if (!IsValid(Attacker) || !IsValid(Defender))
	{
		return false;
	}

	const float AttackRange = ComingAttackPayload.ComingAttack->MaxRange;

	const float Distance = FVector::Dist(
		Attacker->GetActorLocation(),
		Defender->GetActorLocation()
	);

	return Distance <= (AttackRange + 50.f);
}
