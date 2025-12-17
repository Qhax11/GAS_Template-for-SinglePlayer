// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"

bool UMovementDataBase::IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug) const
{
    return true;
}

bool UMovementDataBase::PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug) const
{
    return true;
}

float UMovementDataBase::GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug) const
{
	float Score = 0.f;

	Score += GetDistanceScore(Context);
	Score += GetBehaviorStateScore(Context);
	Score += ScoreBias;

	return Score;
}

float UMovementDataBase::GetDistanceScore(const FMovementDecisionContext& Context) const
{
    return 0.0f;
}

float UMovementDataBase::GetBehaviorStateScore(const FMovementDecisionContext& Context) const
{
    return 0.0f;
}
