// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/BDS_GetBestMovementChain.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"

void UBDS_GetBestMovementChain::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

UMovementDataBase* UBDS_GetBestMovementChain::GetBestMovement(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    if (!SelectedAbilityClass || !MovementChainAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectedAbilityClass or AttackAbilityMovementChainMapAsset is null in: %s!"), *GetName());
        return nullptr;
    }

    FMovementScoreDebug BestScoreDebug;
    UMovementDataBase* BestMovementData = nullptr;
    float BestScore = -FLT_MAX;

    /*
    TArray<UMovementChainAsset*> AbilityMovementChainAssets = GetMovementChainsForSelectedAttackAbility(SelectedAbilityClass);
    if (AbilityMovementChainAssets.IsEmpty())
    {
        return nullptr;
    }
    */

    FMovementDecisionContext MovementDecisionContext;

    for (UMovementDataBase* Movement : MovementChainAsset->MovementAbilitiesData)
    {
        // ---------------- ENABLE CHECK ----------------
        FMovementEnableDebug EnableDebug;
        if (!Movement->IsEnable(MovementDecisionContext, bEnableDebug ? &EnableDebug : nullptr))
        {
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("UBDS_ComingAttackReactionBase: ReactionDisabled = %s | Reason = %s |"),
                    *Movement->MovementName.ToString(),
                    *UEnum::GetValueAsString(EnableDebug.DisableReason)
                );
            }
            continue;
        }

        // ---------------- CHANCE CHECK ----------------
        FMovementChanceDebug ChanceDebug;
        if (!Movement->PassesChance(MovementDecisionContext, bEnableDebug ? &ChanceDebug : nullptr))
        {
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("UBDS_ComingAttackReactionBase: ReactionRollFailed = %s | Reason = %s | Roll = %.2f Threshold = %.2f |"),
                    *Movement->MovementName.ToString(),
                    *UEnum::GetValueAsString(ChanceDebug.ChanceFailReason),
                    ChanceDebug.Roll,
                    ChanceDebug.Threshold
                );
            }
            continue;
        }

        // ---------------- SCORE ----------------
        FMovementScoreDebug ScoreDebug;
        const float Score = Movement->GetScore(MovementDecisionContext, bEnableDebug ? &ScoreDebug : nullptr);
        if (bEnableDebug)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("UBDS_ComingAttackReactionBase: ReactionScore = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
                *Movement->MovementName.ToString(),
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
            BestMovementData = Movement;
            BestScoreDebug = ScoreDebug;
        }
    }

    // ---------------- WINNER DEBUG ----------------
    if (bEnableDebug && BestMovementData)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UBDS_ComingAttackReactionBase: WINNER = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
            *BestMovementData->MovementName.ToString(),
            BestScoreDebug.BaseScore,
            BestScoreDebug.DistanceScore,
            BestScoreDebug.BehaviorStateScore,
            BestScoreDebug.TotalScore
        );
    }

   // ApplyDirectionPoliciesToSelectedMovementChain(BestMovementChainDataAsset);

    return BestMovementData;
}
/*

TArray<UMovementChainAsset*> UBDS_GetBestMovementChain::GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const
{
    TArray<UMovementChainAsset*> Result;

    if (!IsValid(AttackAbilityMovementChainMapAsset) || !SelectedAbilityClass)
    {
        return Result;
    }

    for (const FAttackAbilityMovementChains& Mapping : AttackAbilityMovementChainMapAsset->AttackAbilityMovementChainMap)
    {
        if (Mapping.AttackAbilityClass == SelectedAbilityClass)
        {
            Result.Append(Mapping.);
            break;
        }
    }
    return Result;
}

float UBDS_GetBestMovementChain::CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset)
{
    float Score = 0.0f;


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
    return Score;
}

float UBDS_GetBestMovementChain::CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset)
{
    float Score = 0.0f;



    if (!EnemyController) 
    {
		return Score;
    }

    if (MovementChainAsset->DistanceScoreCurve)
    {
        float CurveScore = MovementChainAsset->DistanceScoreCurve->GetFloatValue(EnemyController->GetTargetHeroDistance());
        Score += CurveScore;
    }

    return Score;
}

float UBDS_GetBestMovementChain::CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset)
{
    float Score = 0.0f;

    if (!MovementChainAsset)
    {
        return Score;
    }

    if (const float* FoundScore = MovementChainAsset->BehaviorStateModifiers.Find(BehaviorState))
    {
        Score += *FoundScore;
    }
    return Score;
}

bool UBDS_GetBestMovementChain::ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset)
{
    bool bChanged = false;


    if (!SelectedMovementChainAsset || !HeroMovementListenerComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectedMovementChainAsset is null in: %s"), *GetName());
        return false;
    }


    FGameplayTag HeroLastDirectionGameplayTag = HeroMovementListenerComp->GetHeroLastMovementDirectionTagByLastInput();

    for (FMovementAbilityData& MovementAbilityInChain : SelectedMovementChainAsset->MovementChain)
    {
        if (!MovementAbilityInChain.DirectionPolicyTag.IsValid())
        {
            continue;
        }

        if (MovementAbilityInChain.DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_PlayerLastDirection)
        {
            if (HeroLastDirectionGameplayTag.IsValid())
            {
                MovementAbilityInChain.ResolvedDirectionTag = HeroLastDirectionGameplayTag;
                bChanged = true;
            }
        }
        else if (MovementAbilityInChain.DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_Random)
        {
            MovementAbilityInChain.ResolvedDirectionTag = GetRandomDirectionTag();
            bChanged = true;
        }
    }

    return bChanged;
}
*/
FGameplayTag UBDS_GetBestMovementChain::GetRandomDirectionTag()
{
    static const TArray<FGameplayTag> PossibleDirections =
    {
        //GAS_Tags::TAG_AI_Direction_Resolved_Forward,
        GAS_Tags::TAG_Gameplay_Direction_Backward,
        GAS_Tags::TAG_Gameplay_Direction_Left,
        GAS_Tags::TAG_Gameplay_Direction_Right
    };

    int32 RandomIndex = FMath::RandRange(0, PossibleDirections.Num() - 1);
    return PossibleDirections[RandomIndex];
}

