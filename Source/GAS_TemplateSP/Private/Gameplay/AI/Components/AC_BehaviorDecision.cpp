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
    if (!AttackAbilityDataAsset || !OwnerEnemyASC)
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
    if (!MovementDataAsset || !OwnerEnemyASC)
    {
        return FMovementData();
    }

    float BestScore = -FLT_MAX;
    FMovementData BestMovement;

    float BestMovementDistanceScore = 0.f;
    float BestMovementTargetMovementScore = 0.f;
    float BestMovementChainScore = 0.f;
    for (const FMovementData& Movement : MovementDataAsset->Movements)
    {
        bool bIsOnCooldown = OwnerEnemyASC->HasMatchingGameplayTag(Movement.MovementCooldownTag);
        if (bIsOnCooldown)
        {
            continue;
        }

        float DistanceScore = CalculateMovementScoreBasedOnTargetDistance(DistanceToTarget, Movement, SelectedAttackAbilityData);
        float TargetMovementScore = CalculateMovementScoreBasedOnTargetMovement(Movement, SelectedAttackAbilityData);
        float ChainScore = CalculateMovementChainScoreBasedOnLastSelectedMovement(Movement, LastSelectedMovementData);

        float TotalScore = Movement.ScoreBias + DistanceScore + TargetMovementScore + ChainScore;

        if (GEngine && EnableAllDataDebug)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Green,
                FString::Printf(TEXT(">> Movement: %s | DistanceScore: %.1f | TargetMovementScore: %.1f | ChainScore: %.1f "),
                    *BestMovement.MovementName.ToString(), BestMovementDistanceScore, BestMovementTargetMovementScore, BestMovementChainScore));
        }

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestMovementDistanceScore = DistanceScore;
            BestMovementTargetMovementScore = TargetMovementScore;
            BestMovementChainScore = ChainScore;
            BestMovement = Movement;
        }
    }

    if (GEngine && EnableSelectedDebug)
    {
        GEngine->AddOnScreenDebugMessage(10, 3.5f, FColor::Cyan,
            FString::Printf(TEXT(">> Selected Movement: %s | DistanceScore: %.1f | TargetMovementScore: %.1f | ChainScore: %.1f "),
                *BestMovement.MovementName.ToString(), BestMovementDistanceScore, BestMovementTargetMovementScore, BestMovementChainScore));
    }

    LastSelectedMovementData = BestMovement;
    return BestMovement;
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

    if (MovementData.DistanceScoreCurve)
    {
        float CurveScore = MovementData.DistanceScoreCurve->GetFloatValue(DistanceToTarget);
        Score += CurveScore;
    }

    return Score;
}

float UAC_BehaviorDecision::CalculateMovementScoreBasedOnTargetMovement(FMovementData MovementData, FAttackData SelectedAttackAbilityData)
{
    float Score = 0.0f;

    EHeroRelativeDirection HeroDirection = HeroMovementListenerComp->GetHeroLastMovementDirectionByInput();
    float Displacement = HeroMovementListenerComp->GetDisplacementInLastSeconds(SecondsCheckMovement);

    if (MovementData.HeroRelativeDirectionScoreModifiers.Contains(HeroDirection))
    {
        Score += MovementData.HeroRelativeDirectionScoreModifiers[HeroDirection];
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

float UAC_BehaviorDecision::CalculateMovementChainScoreBasedOnLastSelectedMovement(FMovementData MovementData, FMovementData LastMovementData)
{
    // Chain works only on Dashes
    if (LastMovementData.MovementName == NAME_None || LastMovementData.MovementType != EMovementType::Dash)
    {
        return 0.0f;
    }

    float ChainScore = 0.0f;

    if (MovementData.LastMovementDirectionScoreModifiers.Contains(LastMovementData.Direction))
    {
        ChainScore += MovementData.LastMovementDirectionScoreModifiers[LastMovementData.Direction];
    }

    return ChainScore;
}




