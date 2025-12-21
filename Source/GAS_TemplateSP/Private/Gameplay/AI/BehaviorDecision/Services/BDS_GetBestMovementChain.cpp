// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/BDS_GetBestMovementChain.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"
#include "Gameplay/AI/Components/AC_IntendManager.h"

void UBDS_GetBestMovementChain::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

UMovementChainData* UBDS_GetBestMovementChain::GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    if (!SelectedAbilityClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision: Service: UBDS_GetBestMovementChain: SelectedAbilityClass is null in: %s!"), *GetName());
        return nullptr;
    }

    UMovementChainData* BestMovementChainData = nullptr;
    FMovementScoreDebug BestScoreDebug;
    float BestScore = -FLT_MAX;

    // Seçili attack ability için movement chain asset listesini al
    TArray<UMovementChainAsset*> MovementChainAssets = GetMovementChainsForSelectedAttackAbility(SelectedAbilityClass);
    if (MovementChainAssets.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision: Service: UBDS_GetBestMovementChain: No movement chains found for ability: %s!"), *SelectedAbilityClass->GetName());
        return nullptr;
    }

    FMovementDecisionContext MovementDecisionContext;
    MovementDecisionContext.Intent = IntendManager->GetCurrentIntent();
    MovementDecisionContext.Owner = Enemy;
    MovementDecisionContext.Target = Hero;

    for (UMovementChainAsset* ChainAsset : MovementChainAssets)
    {
        if (!ChainAsset || !ChainAsset->MovementChain)
        {
            UE_LOG(LogTemp, Warning, TEXT("Decision: Service: UBDS_GetBestMovementChain: Invalid ChainAsset or MovementChain is null!"));
            continue;
        }

        UMovementChainData* MovementChain = ChainAsset->MovementChain;

        // ---------------- ENABLE CHECK ----------------
        FMovementEnableDebug EnableDebug;
        if (!MovementChain->IsEnable(MovementDecisionContext, bEnableDebug ? &EnableDebug : nullptr))
        {
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("Decision: Service: UBDS_GetBestMovementChain: MovementChainDisabled = %s | Reason = %s |"),
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
                    TEXT("Decision: Service: UBDS_GetBestMovementChain: MovementChainRollFailed = %s | Reason = %s | Roll = %.2f Threshold = %.2f |"),
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
                TEXT("Decision: Service: UBDS_GetBestMovementChain: MovementChainScore = %s | Intent = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
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
                TEXT("Decision: Service: UBDS_GetBestMovementChain: WINNER = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
                *BestMovementChainData->MovementChainName.ToString(),
                BestScoreDebug.BehaviorStateScore,
                BestScoreDebug.DistanceScore,
                BestScoreDebug.BiasScore,
                BestScoreDebug.TotalScore
            );
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Decision: Service: UBDS_GetBestMovementChain: No valid movement chain found!"));
        }
    }

   // ApplyDirectionPoliciesToSelectedMovementChain(BestMovementChainDataAsset);

    return BestMovementChainData;
}

TArray<UMovementChainAsset*> UBDS_GetBestMovementChain::GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const
{
    TArray<UMovementChainAsset*> EmptyResult;

    if (!SelectedAbilityClass || !AttackAbilitiesToMovementChainsAsset)
    {
        return EmptyResult;
    }

    for (const FAttackAbilityToMovementChains& AttackAbilityToMovementChain : AttackAbilitiesToMovementChainsAsset->AttackAbilityMovementChainMap)
    {
        if (AttackAbilityToMovementChain.AttackAbilityClass == SelectedAbilityClass)
        {
            return AttackAbilityToMovementChain.MovementChains;
        }
    }

    return EmptyResult;
}



