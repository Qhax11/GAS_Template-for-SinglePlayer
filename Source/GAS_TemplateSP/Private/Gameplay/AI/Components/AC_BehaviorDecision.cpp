// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/StateTree/ST_Base.h"

UAC_BehaviorDecision::UAC_BehaviorDecision()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_BehaviorDecision::BeginPlay()
{
	Super::BeginPlay();

    OwnerController = Cast<AAIControllerBase>(GetOwner());
    if (!OwnerController) 
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerController is null in: %s !"), *GetName());
    }

    EnemyBase = Cast<AGAS_EnemyBase>(OwnerController->GetPawn());
    if (!EnemyBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyBase is null in: %s !"), *GetName());
    }

    EnemyASC = EnemyBase->GetAbilitySystemComponent();
    if (!EnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyASC is null in: %s !"), *GetName());
    }
}

FAttackData UAC_BehaviorDecision::GetBestAttack(float DistanceToTarget)
{
    if (!AttackAbilityDataAsset)
    {
        return FAttackData();
    }

    float BestScore = -FLT_MAX;
    FAttackData BestAttack;

    for (const FAttackData& Attack : AttackAbilityDataAsset->AttackAbilities)
    {
        if (!Attack.AbilityClass) 
        {
            continue;
        }

        bool bIsOnCooldown = EnemyASC->HasMatchingGameplayTag(Attack.AbilityCooldownTag);
        bool bWasBlockedRecently = false; // dış sistemden okunmalı

        float DistanceScore = CalculateAttackAbilityDistanceScore(DistanceToTarget, Attack.MinRange, Attack.MaxRange);

        float TotalScore = Attack.ScoreBias + DistanceScore;
        if (bIsOnCooldown) 
        {
            TotalScore = 0;
        }

        UE_LOG(LogTemp, Log, TEXT("[AI] Attack %s → Score: %.2f"), *Attack.AbilityClass->GetName(), TotalScore);

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestAttack = Attack;
        }
    }

    LastSelectedAttackAbilityData = BestAttack;
    return BestAttack;
}

FMovementData UAC_BehaviorDecision::GetBestMovement(float DistanceToTarget, FAttackData SelectedAttackAbilityData)
{
    if (!MovementDataAsset)
    {
        return FMovementData();
    }

    float BestScore = -FLT_MAX;
    FMovementData BestMovement;
    
    for (const FMovementData& Movement : MovementDataAsset->Movements)
    {
        // DistanceScore
        float DistanceScore = CalculateMovementDistanceScore(DistanceToTarget, Movement, SelectedAttackAbilityData);

        float TotalScore = Movement.ScoreBias + DistanceScore;

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestMovement = Movement;
        }
    }
    LastSelectedMovementyData = BestMovement;
    return LastSelectedMovementyData;
}

float UAC_BehaviorDecision::CalculateAttackAbilityDistanceScore(float DistanceToTarget, float AbilityMinRange, float AbilityMaxRange)
{
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

float UAC_BehaviorDecision::CalculateMovementDistanceScore(float DistanceToTarget, FMovementData MovementData, FAttackData SelectedAttackAbilityData)
{
    float Score = 0.0f;

    if (DistanceToTarget < 300.f && MovementData.MovementType == EMovementType::Walk)
    {
        Score += 1.0f;
    }
    else if (DistanceToTarget >= 300.f && DistanceToTarget <= 800.f && MovementData.MovementType == EMovementType::Run)
    {
        Score += 0.8f;
    }
    else if (DistanceToTarget > 800.f && MovementData.MovementType == EMovementType::Dash)
    {
        Score += 1.0f;
    }

    return Score;
}





