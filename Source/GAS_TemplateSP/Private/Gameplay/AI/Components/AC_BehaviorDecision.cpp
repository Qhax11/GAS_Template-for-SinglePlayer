// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/StateTree/ST_Base.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

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
        return;
    }

    OwnerEnemyBase = Cast<AGAS_EnemyBase>(OwnerController->GetPawn());
    if (!OwnerEnemyBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyBase is null in: %s !"), *GetName());
        return;
    }

    OwnerEnemyASC = OwnerEnemyBase->GetAbilitySystemComponent();
    if (!OwnerEnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyASC is null in: %s !"), *GetName());
        return;
    }

    OwnerController->OnTargetDetected.AddDynamic(this, &UAC_BehaviorDecision::OnTargetDetected);
}

void UAC_BehaviorDecision::OnTargetDetected(AActor* Target)
{
    HeroBase = Cast<AGAS_HeroBase>(Target);
    if (!HeroBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s !"), *GetName());
        return;
    }

    HeroMovementListenerComp = HeroBase->GetMovementListenerComponent();
    if (!HeroMovementListenerComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroMovementListenerComp is null in: %s !"), *GetName());
        return;
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

        bool bIsOnCooldown = OwnerEnemyASC->HasMatchingGameplayTag(Attack.AbilityCooldownTag);
        if (bIsOnCooldown) 
        {
            continue;
        }

        bool bWasBlockedRecently = false; // dış sistemden okunmalı

        float DistanceScore = CalculateAttackAbilityScoreBasedOnTargetDistance(DistanceToTarget, Attack.MinRange, Attack.MaxRange);

        float TotalScore = Attack.ScoreBias + DistanceScore;

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

    float DistanceScore = 0.f;
    float TargetMovementScore = 0.f;
    for (const FMovementData& Movement : MovementDataAsset->Movements)
    {
        DistanceScore = CalculateMovementScoreBasedOnTargetDistance(DistanceToTarget, Movement, SelectedAttackAbilityData);
        TargetMovementScore = CalculateMovementScoreBasedOnTargetMovement(Movement, SelectedAttackAbilityData);

        float TotalScore = Movement.ScoreBias + DistanceScore + TargetMovementScore;

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestMovement = Movement;
        }
    }

    if (GEngine && EnableDebug)
    {
        GEngine->AddOnScreenDebugMessage(10, 3.5f, FColor::Cyan,
            FString::Printf(TEXT(">> Selected Movement: %s | DistanceScore: %.1f | TargetMovementScore: %.1f"),
                *BestMovement.MovementName.ToString(), DistanceScore, TargetMovementScore));
    }

    LastSelectedMovementyData = BestMovement;
    return LastSelectedMovementyData;
}

float UAC_BehaviorDecision::CalculateAttackAbilityScoreBasedOnTargetDistance(float DistanceToTarget, float AbilityMinRange, float AbilityMaxRange)
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

float UAC_BehaviorDecision::CalculateMovementScoreBasedOnTargetDistance(float DistanceToTarget, FMovementData MovementData, FAttackData SelectedAttackAbilityData)
{
    float Score = 0.0f;


    // If the distance is shorter than the attack's minimum range, moving backward helps to increase distance and reach effective range.
    if ((DistanceToTarget < SelectedAttackAbilityData.MinRange) && (MovementData.Direction == EMovementDirection::Backward))
    {
        Score += 1.0f;
    }

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

float UAC_BehaviorDecision::CalculateMovementScoreBasedOnTargetMovement(FMovementData MovementData, FAttackData SelectedAttackAbilityData)
{
    float Score = 0.0f;

    EHeroRelativeDirectionToTarget HeroDirection = HeroMovementListenerComp->GetRelativeMovementDirection(2, OwnerEnemyBase);
    float Displacement = HeroMovementListenerComp->GetDisplacementInLastSeconds(2);

    if (MovementData.HeroRelativeDirectionToTargetScoreModifiers.Contains(HeroDirection))
    {
        Score += MovementData.HeroRelativeDirectionToTargetScoreModifiers[HeroDirection];
    }

    // 2. Oyuncu gerçekten anlamlı bir şekilde hareket etti mi?
    if (Displacement > 50.f) // örnek eşik değeri, ayarlanabilir
    {
        // Daha hareketli bir oyuncuya göre bazı hareket türleri (örneğin Dash) tercih edilebilir
        Score += MovementData.ScoreModifierWhenTargetIsMoving;
    }
    else
    {
        Score += MovementData.ScoreModifierWhenTargetIsIdle;
    }

    return Score;
}




