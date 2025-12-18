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
        UE_LOG(LogTemp, Warning, TEXT("UBDS_GetBestMovementChain: SelectedAbilityClass is null in: %s!"), *GetName());
        return nullptr;
    }

    UMovementChainDataa* BestMovementChainData = nullptr;
    FMovementScoreDebug BestScoreDebug;
    float BestScore = -FLT_MAX;

    UMovementChainsAsset* SelectedAbilityMovementChains = GetMovementChainsForSelectedAttackAbility(SelectedAbilityClass);
    if (!SelectedAbilityMovementChains)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBDS_GetBestMovementChain: SelectedAbilityMovementChains is null in: %s!"), *GetName());
        return nullptr;
    }

    FMovementDecisionContext MovementDecisionContext;
    MovementDecisionContext.BehaviorState = BehaviorState;
    MovementDecisionContext.Owner = Enemy;
    MovementDecisionContext.Target = Hero;

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
                    *MovementChain->MovementChainName.ToString(),
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
                    *MovementChain->MovementChainName.ToString(),
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
                *MovementChain->MovementChainName.ToString(),
                ScoreDebug.BehaviorStateScore,
                ScoreDebug.DistanceScore,
                ScoreDebug.BiasScore,
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
    if (bEnableDebug)
    {
        if (BestMovementChainData) 
        {
            UE_LOG(LogTemp, Warning,
                TEXT("UBDS_GetBestMovementChain: WINNER = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
                *BestMovementChainData->MovementChainName.ToString(),
                BestScoreDebug.BehaviorStateScore,
                BestScoreDebug.DistanceScore,
                BestScoreDebug.BiasScore,
                BestScoreDebug.TotalScore
            );
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UBDS_GetBestMovementChain: No valid movement chain found!"));
        }
    }

   // ApplyDirectionPoliciesToSelectedMovementChain(BestMovementChainDataAsset);

    return BestMovementChainData;
}

UMovementChainsAsset* UBDS_GetBestMovementChain::GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const
{
    if (!SelectedAbilityClass || !AttackAbilitiesToMovementChainsAsset)
    {
        return nullptr;
    }

    for (const FAttackAbilityToMovementChain& AttackAbilityToMovementChain : AttackAbilitiesToMovementChainsAsset->AttackAbilityMovementChainMap)
    {
        if (AttackAbilityToMovementChain.AttackAbilityClass == SelectedAbilityClass)
        {
            return AttackAbilityToMovementChain.MovementChainsAsset;
        }
    }

    return nullptr;
}



