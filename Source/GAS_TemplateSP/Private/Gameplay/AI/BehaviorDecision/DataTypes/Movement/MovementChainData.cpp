// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainData.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

bool UMovementChainData::IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug) const
{
    if (!Context.Owner || !Context.Target)
    {
        if (OutDebug)
        {
            OutDebug->DisableReason = EMovementChainDisableReason::InvalidContext;
        }
        return false;
    }

    if (Context.bIsAnyMovementAbilityOnCooldown)
    {
        if (OutDebug)
        {
            OutDebug->DisableReason = EMovementChainDisableReason::OnCooldown;
        }
        return false;
    }

    if (OutDebug)
    {
        OutDebug->DisableReason = EMovementChainDisableReason::None;
    }

    return true;
}

bool UMovementChainData::PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug) const
{
    return true;
}

float UMovementChainData::GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug) const
{
    float DistanceScore = GetDistanceScore(Context);
    float BehaviorScore = GetBehaviorStateScore(Context);

    const float TotalScore = DistanceScore + BehaviorScore +ScoreBias;

    if (OutDebug)
    {
        OutDebug->BiasScore = ScoreBias;
        OutDebug->DistanceScore = DistanceScore;
        OutDebug->BehaviorStateScore = BehaviorScore;
        OutDebug->TotalScore = TotalScore;
    }

    return TotalScore;
}

float UMovementChainData::GetBehaviorStateScore(const FMovementDecisionContext& Context) const
{
    float Score = 0.0f;

    if (const float* FoundScore = BehaviorStateModifiers.Find(Context.BehaviorState))
    {
        Score += *FoundScore;
    }

    return Score;
}

float UMovementChainData::GetDistanceScore(const FMovementDecisionContext& Context) const
{
    float Score = 0.0f;

    if (DistanceScoreCurve)
    {
        const float Distance = CombatDistance::GetDistance(Context.Owner, Context.Target);
        Score += DistanceScoreCurve->GetFloatValue(Distance);
    }

    return Score;
}

float UMovementChainData::GetTargetMovementScore(const FMovementDecisionContext& Context) const
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

