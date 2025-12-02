// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/BDS_GetBestAttack.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"

void UBDS_GetBestAttack::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

FAttackData UBDS_GetBestAttack::GetBestAttack()
{
    if (!IsValid(AttackAbilityAsset) || !EnemyASC)
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

        float DistanceScore = CalculateAttackAbilityScoreBasedOnTargetDistance(Attack, EnemyController->GetTargetHeroDistance());

        float ComboScore = CalculateComboScore(Attack);

        float TotalScore = Attack.ScoreBias + DistanceScore + ComboScore;

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
    */

    LastSelectedAttackAbilityData = BestAttack;
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
        Score = -100; // Çok yakınsa etkisizleştir
    }

    return Score;
}

float UBDS_GetBestAttack::CalculateComboScore(FAttackData AttackData)
{
    // If there is no valid last attack or it wasn't part of a combo chain
    if (!LastSelectedAttackAbilityData.AbilityClass || !LastSelectedAttackAbilityData.bIsComboAttack)
    {
        return 0.0f;
    }

    // If the current candidate isn't a combo attack, skip
    if (!AttackData.bIsComboAttack)
    {
        return 0.0f;
    }

    // Expected combo index is always the next step after the last selected
    int32 ExpectedNextIndex = LastSelectedAttackAbilityData.ComboIndex + 1;

    // If this attack matches the expected combo step, give it a strong score
    if (AttackData.ComboIndex == ExpectedNextIndex)
    {
        return 3.0f;
    }
    
    return 0.0f;
}

