// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainDataa.h"

bool UMovementChainDataa::IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug) const
{
    return false;
}

bool UMovementChainDataa::PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug) const
{
    return false;
}

float UMovementChainDataa::GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug) const
{
    return 0.0f;
}

float UMovementChainDataa::GetDistanceScore(const FMovementDecisionContext& Context) const
{
    float Score = 0.0f;
    /*
    Context.

    if (!EnemyController)
    {
        return Score;
    }

    if (MovementChainAsset->DistanceScoreCurve)
    {
        float CurveScore = MovementChainAsset->DistanceScoreCurve->GetFloatValue(EnemyController->GetTargetHeroDistance());
        Score += CurveScore;
    }
    */
    return Score;


}

float UMovementChainDataa::GetTargetMovementScore(const FMovementDecisionContext& Context) const
{
    float Score = 0.0f;

    /*

    if (!HeroMovementListenerComp || !EnemyController)
    {
        return Score;
    }

    const float HeroDisplacement = HeroMovementListenerComp->GetDisplacementInLastSeconds(2.0f);

    if (HeroDisplacement > 50.0f)
    {
        Score += MovementChainAsset->ScoreModifierWhenTargetIsMoving;
    }
    else
    {
        Score += MovementChainAsset->ScoreModifierWhenTargetIsNotMoving;
    }

    // Penalty
    if (EnemyController->GetTargetHeroDistance() < MovementChainAsset->MinRange)
    {
        Score = -100.0f;
    }
    */

    return Score;
}

float UMovementChainDataa::GetBehaviorStateScore(const FMovementDecisionContext& Context) 
{
    float Score = 0.0f;

    if (const float* FoundScore = BehaviorStateModifiers.Find(Context.BehaviorState))
    {
        Score += *FoundScore;
    }
    return Score;
}
