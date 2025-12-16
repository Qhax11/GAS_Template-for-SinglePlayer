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

        FReactionEnableDebug EnableDebug;
        if (!Reaction->IsEnable(ComingAttackPayload, bEnableDebug ? &EnableDebug : nullptr))
        {
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("UBDS_ComingAttackReactionBase: ReactionDisabled =  %s | Reason=%s"),
                    *Reaction->GetName(),
                    *UEnum::GetValueAsString(EnableDebug.Reason)
                );
            }
            continue;
        }

        FReactionChanceDebug ChanceDebug;
        if (!Reaction->PassesChanceRoll(EnemyASC, bEnableDebug ? &ChanceDebug : nullptr))
        {
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("UBDS_ComingAttackReactionBase: ReactionChanceFailed = %s | Reason=%s | Roll=%.2f Threshold=%.2f"),
                    *Reaction->GetName(),
                    *UEnum::GetValueAsString(ChanceDebug.Reason),
                    ChanceDebug.Roll,
                    ChanceDebug.Threshold
                );
            }
            continue;
        }

        FReactionScoreDebug Debug;
        const float Score = Reaction->GetScore(ComingAttackPayload, BehaviorState, &Debug);
        if (bEnableDebug)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("UBDS_ComingAttackReactionBase: ReactionScore = %s | Behavior=%.2f Tag=%.2f Bias=%.2f Total=%.2f"),
                *Reaction->GetName(),
                Debug.BehaviorStateScore,
                Debug.TagScore,
                Debug.Bias,
                Debug.Total
            );
        }

        if (Score > BestScore)
        {
            BestScore = Score;
            BestReaction = Reaction;
        }

    }

    return BestReaction;
}







