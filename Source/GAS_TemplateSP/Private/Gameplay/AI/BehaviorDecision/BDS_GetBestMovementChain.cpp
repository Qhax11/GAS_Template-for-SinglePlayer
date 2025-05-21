// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/BDS_GetBestMovementChain.h"

void UBDS_GetBestMovementChain::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);

    UAttackAbilityMovementChainMapAsset* CastedAsset = Cast<UAttackAbilityMovementChainMapAsset>(BehaviorServiceInitParams.Asset);
    if (CastedAsset)
    {
        AttackAbilityMovementChainMapAsset = CastedAsset;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid asset type passed to %s!"), *GetName());
    }
}

UMovementChainAsset* UBDS_GetBestMovementChain::GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    if (!SelectedAbilityClass || !AttackAbilityMovementChainMapAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectedAbilityClass or AttackAbilityMovementChainMapAsset is null in: %s!"), *GetName());
        return nullptr;
    }

    UMovementChainAsset* BestMovementChainDataAsset = nullptr;

    float BestScore = -FLT_MAX;
    float BestMovementChainDistanceScore = 0.f;
    float BestMovementChainTargetMovementScore = 0.f;

    TArray<UMovementChainAsset*> AbilityMovementChainAssets = GetMovementChainsForSelectedAttackAbility(SelectedAbilityClass);
    if (AbilityMovementChainAssets.IsEmpty())
    {
        return nullptr;
    }

    for (UMovementChainAsset* MovementChainAsset : AbilityMovementChainAssets)
    {
        if (!MovementChainAsset) 
        {
            continue;
        }

        if (GetTargetDistance() < MovementChainAsset->MinRange)
        {
            continue;
        }

        float DistanceScore = CalculateMovementChainScoreBasedOnTargetDistance(MovementChainAsset);
        float TargetMovementScore = CalculateMovementChainScoreBasedOnTargetMovement(MovementChainAsset);
        float BehaviorStateScore = CalculateMovementChainScoreBasedOnBehaviorState(MovementChainAsset);

        float TotalScore = MovementChainAsset->ScoreBias + DistanceScore + TargetMovementScore + BehaviorStateScore;

        UE_LOG(LogTemp, Log, TEXT("[AI] MovementChain %s → Score: %.2f"), *MovementChainAsset->MovementChainName.ToString(), TotalScore);

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestMovementChainDistanceScore = DistanceScore;
            BestMovementChainTargetMovementScore = TargetMovementScore;
            BestMovementChainDataAsset = MovementChainAsset;
        }
    }

    ApplyDirectionPoliciesToSelectedMovementChain(BestMovementChainDataAsset);
    /*
    if (GEngine && EnableSelectedDebug)
    {
        GEngine->AddOnScreenDebugMessage(10, 3.5f, FColor::Cyan,
            FString::Printf(TEXT(">> Selected MovementChain: %s | DistanceScore: %.1f | TargetMovementScore: %.1f "),
                *BestMovementChainDataAsset->MovementChainName.ToString(), BestMovementChainDistanceScore, BestMovementChainTargetMovementScore));
    }
    */
    return BestMovementChainDataAsset;
}

TArray<UMovementChainAsset*> UBDS_GetBestMovementChain::GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const
{
    TArray<UMovementChainAsset*> Result;

    if (!IsValid(AttackAbilityMovementChainMapAsset) || !SelectedAbilityClass)
    {
        return Result;
    }

    for (const FAttackAbilityMovementChains& Mapping : AttackAbilityMovementChainMapAsset->ChainMappings)
    {
        if (Mapping.AttackAbilityClass == SelectedAbilityClass)
        {
            Result.Append(Mapping.MovementChainAssets);
            break;
        }
    }

    return Result;
}

float UBDS_GetBestMovementChain::CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset)
{
    float Score = 0.0f;

    const float HeroDisplacement = HeroMovementListenerComp->GetDisplacementInLastSeconds(2.0f);

    if (HeroDisplacement > 50.0f)
    {
        Score += MovementChainAsset->ScoreModifierWhenTargetIsMoving;
    }
    else
    {
        Score += MovementChainAsset->ScoreModifierWhenTargetIsNotMoving;
    }

    return Score;
}

float UBDS_GetBestMovementChain::CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset)
{
    float Score = 0.0f;

    if (MovementChainAsset->DistanceScoreCurve)
    {
        float CurveScore = MovementChainAsset->DistanceScoreCurve->GetFloatValue(GetTargetDistance());
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
    if (!SelectedMovementChainAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectedMovementChainAsset is null in: %s"), *GetName());
        return false;
    }
    bool bChanged = false;

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

FGameplayTag UBDS_GetBestMovementChain::GetRandomDirectionTag()
{
    static const TArray<FGameplayTag> PossibleDirections =
    {
        //GAS_Tags::TAG_AI_Direction_Resolved_Forward,
        GAS_Tags::TAG_AI_Direction_Resolved_Backward,
        GAS_Tags::TAG_AI_Direction_Resolved_Left,
        GAS_Tags::TAG_AI_Direction_Resolved_Right
    };

    int32 RandomIndex = FMath::RandRange(0, PossibleDirections.Num() - 1);
    return PossibleDirections[RandomIndex];
}
