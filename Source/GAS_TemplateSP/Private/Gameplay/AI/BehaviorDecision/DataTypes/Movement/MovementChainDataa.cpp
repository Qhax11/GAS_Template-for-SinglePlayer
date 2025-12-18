// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainDataa.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

bool UMovementChainDataa::IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug) const
{
    if (!Context.Owner || !Context.Target)
    {
        if (OutDebug)
        {
            OutDebug->DisableReason = EMovementChainDisableReason::InvalidContext;
        }
        return false;
    }

    if (!IsDistanceAllowed(Context))
    {
        if (OutDebug)
        {
            OutDebug->DisableReason = EMovementChainDisableReason::DistanceNotAllowed;
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

bool UMovementChainDataa::IsDistanceAllowed(const FMovementDecisionContext& Context) const
{
    // 0 veya negatif = range kýsýtý yok
    if (MinRange <= 0.f)
    {
        return true;
    }

    return CombatDistance::IsInRange(Context.Owner, Context.Target, MinRange);
}

bool UMovementChainDataa::PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug) const
{
    return true;
}

float UMovementChainDataa::GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug) const
{
    float DistanceScore = GetDistanceScore(Context);
    float BehaviorScore = 0.f;

    // Behavior state modifier
    if (const float* Modifier = BehaviorStateModifiers.Find(Context.BehaviorState))
    {
        BehaviorScore = *Modifier;
    }

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
