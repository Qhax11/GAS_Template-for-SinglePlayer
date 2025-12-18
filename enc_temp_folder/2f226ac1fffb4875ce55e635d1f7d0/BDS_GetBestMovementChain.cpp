// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/BDS_GetBestMovementChain.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainDataa.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"

void UBDS_GetBestMovementChain::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

UMovementChainDataa* UBDS_GetBestMovementChain::GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    if (!SelectedAbilityClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectedAbilityClass or AttackAbilityMovementChainMapAsset is null in: %s!"), *GetName());
        return nullptr;
    }

    UMovementChainDataa* BestMovementChainData = nullptr;
    FMovementScoreDebug BestScoreDebug;
    float BestScore = -FLT_MAX;

    UMovementChainsAsset* SelectedAbilityMovementChains = GetMovementChainsForSelectedAttackAbility(SelectedAbilityClass);
    if (!SelectedAbilityMovementChains)
    {
        return nullptr;
    }

    FMovementDecisionContext MovementDecisionContext;

    for (UMovementChainDataa* MovementChain : SelectedAbilityMovementChains->MovementChains)
    {
        if (!MovementChain)
        {
            UE_LOG(LogTemp, Warning, TEXT("UBDS_GetBestMovementChain: MovementChain is null!"));
            continue;
        }

        // ---------------- ENABLE CHECK ----------------
        FMovementEnableDebug EnableDebug;
        if (!MovementChain->IsEnable(MovementDecisionContext, bEnableDebug ? &EnableDebug : nullptr))
        {
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("UBDS_GetBestMovementChain: MovementChainDisabled = %s | Reason = %s |"),
                    *MovementChain->MovementName.ToString(),
                    *UEnum::GetValueAsString(EnableDebug.DisableReason)
                );
            }
            continue;
        }

        // ---------------- CHANCE CHECK ----------------
        FMovementChanceDebug ChanceDebug;
        if (!MovementChain->PassesChance(MovementDecisionContext, bEnableDebug ? &ChanceDebug : nullptr))
        {
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("UBDS_GetBestMovementChain: MovementChainRollFailed = %s | Reason = %s | Roll = %.2f Threshold = %.2f |"),
                    *MovementChain->MovementName.ToString(),
                    *UEnum::GetValueAsString(ChanceDebug.ChanceFailReason),
                    ChanceDebug.Roll,
                    ChanceDebug.Threshold
                );
            }
            continue;
        }

        // ---------------- SCORE ----------------
        FMovementScoreDebug ScoreDebug;
        const float Score = MovementChain->GetScore(MovementDecisionContext, bEnableDebug ? &ScoreDebug : nullptr);
        if (bEnableDebug)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("UBDS_GetBestMovementChain: MovementChainScore = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
                *MovementChain->MovementName.ToString(),
                ScoreDebug.BaseScore,
                ScoreDebug.DistanceScore,
                ScoreDebug.BehaviorStateScore,
                ScoreDebug.TotalScore
            );
        }

        // ---------------- BEST PICK ----------------
        if (Score > BestScore)
        {
            BestScore = Score;
            BestMovementChainData = MovementChain;
            BestScoreDebug = ScoreDebug;
        }
    }

    // ---------------- WINNER DEBUG ----------------
    if (bEnableDebug && BestMovementChainData)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UBDS_GetBestMovementChain: WINNER = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
            *BestMovementChainData->MovementName.ToString(),
            BestScoreDebug.BaseScore,
            BestScoreDebug.DistanceScore,
            BestScoreDebug.BehaviorStateScore,
            BestScoreDebug.TotalScore
        );
    }

   // ApplyDirectionPoliciesToSelectedMovementChain(BestMovementChainDataAsset);

    return BestMovementChainData;
}

UMovementChainsAsset* UBDS_GetBestMovementChain::GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const
{
    UMovementChainsAsset* Result = nullptr;

    if (!IsValid(AttackAbilitiesToMovementChainsAsset) || !SelectedAbilityClass)
    {
        return Result;
    }

    for (const FAttackAbilityToMovementChain& AttackAbilityToMovementChain : AttackAbilitiesToMovementChainsAsset->AttackAbilityMovementChainMap)
    {
        if (AttackAbilityToMovementChain.AttackAbilityClass == SelectedAbilityClass)
        {
            return AttackAbilityToMovementChain.MovementChainsAsset;
        }
    }
    return Result;
}



