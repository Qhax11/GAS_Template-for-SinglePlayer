// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/BDS_GetBestAttack.h"

void UBDS_GetBestAttack::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);

    UAttackAbilityAsset* CastedAsset = Cast<UAttackAbilityAsset>(BehaviorServiceInitParams.Asset);
    if (CastedAsset)
    {
        AttackAbilityAsset = CastedAsset;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid asset type passed to UDefenseReactionService!"));
    }
}

FAttackData UBDS_GetBestAttack::GetBestAttack(float DistanceToTarget)
{
    if (!AttackAbilityAsset || !EnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackAbilityAsset or OwnerEnemyASC is null in: %s !"), *GetName());
        return FAttackData();
    }

    float BestScore = -FLT_MAX;
    FAttackData BestAttack;
    float BestAttackDistanceScore = 0.f;

    for (const FAttackData& Attack : AttackAbilityAsset->AttackAbilities)
    {
        if (!Attack.AbilityClass)
        {
            continue;
        }

        bool IsInCooldown = Attack.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->IsOnCooldown(EnemyASC);
        if (IsInCooldown)
        {
            continue;
        }

        float DistanceScore = CalculateAttackAbilityScoreBasedOnTargetDistance(Attack, DistanceToTarget);

        float TotalScore = Attack.ScoreBias + DistanceScore;

        UE_LOG(LogTemp, Log, TEXT("[AI] Attack %s → Score: %.2f"), *Attack.AbilityClass->GetName(), TotalScore);

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestAttack = Attack;
        }
    }

    /*
    if (GEngine && EnableSelectedDebug)
    {
        GEngine->AddOnScreenDebugMessage(9, 3.5f, FColor::Red,
            FString::Printf(TEXT(">> Selected Attack: %s | DistanceScore: %.1f"),
                *BestAttack.AbilityClass->GetName(), BestAttackDistanceScore));
    }

    LastSelectedAttackAbilityData = BestAttack;
    */
    return BestAttack;
}

float UBDS_GetBestAttack::CalculateAttackAbilityScoreBasedOnTargetDistance(FAttackData AttackData, float DistanceToTarget)
{
    float AbilityMinRange = AttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->MinRange;
    float AbilityMaxRange = AttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>()->MaxRange;
    if (AbilityMaxRange <= 0.f)
    {
        return 0.0f;
    }

    // Saldırının ideal noktası: MaxRange
    float DistanceFromIdeal = FMath::Abs(DistanceToTarget - AbilityMaxRange);

    // Skoru mesafeye göre ters orantılı olarak hesapla
    float Score = 1.f - (DistanceFromIdeal / AbilityMaxRange);

    // Minimum Range'in ALTINDA mesafedeyse ekstra ceza uygula (isteğe bağlı)
    if (DistanceToTarget < AbilityMinRange)
    {
        Score *= 0.5f; // Çok yakınsa etkisizleştir
    }

    return FMath::Clamp(Score, 0.f, 1.f);
}

