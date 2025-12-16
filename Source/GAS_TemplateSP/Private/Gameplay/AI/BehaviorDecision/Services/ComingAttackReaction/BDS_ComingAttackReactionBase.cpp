// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include <Gameplay/Attributes/AS_Base.h>
#include <Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataDodge.h>

void UBDS_ComingAttackReactionBase::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

UComingAttackReactionData* UBDS_ComingAttackReactionBase::GetBestComingAttackReaction(FComingAttackPayload& ComingAttackPayload)
{
    if (!IsValid(ComingAttackReactionAsset) || !ComingAttackPayload.ComingAttack || !EnemyASC)
    {
        return nullptr;
    }

    UComingAttackReactionData* BestReaction = nullptr;
    float BestScore = -FLT_MAX;

    for (UComingAttackReactionData* Reaction : ComingAttackReactionAsset->ComingAttackReactions)
    {
        if (!IsValid(Reaction))
        {
            continue;
        }

        if (!Reaction->IsEnable(ComingAttackPayload))
        {
            continue;
        }

        if (!Reaction->PassesChanceRoll(EnemyASC))
        {
            continue;
        }

        FReactionScoreDebug Debug;
        const float Score = Reaction->GetScore(ComingAttackPayload, BehaviorState, &Debug);
        if (Score > BestScore)
        {
            BestScore = Score;
            BestReaction = Reaction;
        }

        if (bEnableDebug)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("[ReactionScore] %s | Behavior=%.2f Tag=%.2f Bias=%.2f Total=%.2f"),
                *Reaction->GetName(),
                Debug.BehaviorStateScore,
                Debug.TagScore,
                Debug.Bias,
                Debug.Total
            );
        }

    }

    return BestReaction;
}







