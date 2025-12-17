// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"

bool UMovementDataBase::IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug) const
{
    return false;
}

bool UMovementDataBase::PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug) const
{
    return false;
}

float UMovementDataBase::GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug) const
{
    return 0.0f;
}
