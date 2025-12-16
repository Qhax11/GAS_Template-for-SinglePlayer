// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"


bool UComingAttackReactionData::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
	return true;
}

bool UComingAttackReactionData::PassesChanceRoll(const UAbilitySystemComponent* ASC) const
{
	const float Roll = FMath::FRandRange(0.f, 1.f);
	return Roll <= BaseChance;
}

float UComingAttackReactionData::GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState) const
{
	float BehaviorStateScore = CalculateBehaviorStateScore(ComingAttackPayload, BehaviorState);
	float TagScore = CalculateTagScore(ComingAttackPayload);
	return BehaviorStateScore + TagScore + ScoreBias;
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
