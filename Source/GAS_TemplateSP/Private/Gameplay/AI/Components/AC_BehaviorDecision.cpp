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
    if (!AttackAbilityAsset || !OwnerEnemyASC)
    {
        return FAttackData();
    }

    float BestScore = -FLT_MAX;
    FAttackData BestAttack;

    for (const FAttackData& Attack : AttackAbilityAsset->AttackAbilities)
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

TArray<FMovementAbilityData> UAC_BehaviorDecision::GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    if (!SelectedAbilityClass || !AttackAbilityMovementChainMapAsset)
    {
        return TArray<FMovementAbilityData>();
    }

    UMovementChainAsset* BestMovementChainDataAsset = nullptr;

    float BestScore = -FLT_MAX;
    float BestMovementChainDistanceScore = 0.f;
    float BestMovementChainTargetMovementScore = 0.f;

    TArray<UMovementChainAsset*> AbilityMovementChainAssets = GetMovementChainsForSelectedAttackAbility(SelectedAbilityClass);
    for (UMovementChainAsset* MovementChainAsset : AbilityMovementChainAssets)
    {
        float DistanceScore = CalculateMovementChainScoreBasedOnTargetDistance(MovementChainAsset);
        float TargetMovementScore = CalculateMovementChainScoreBasedOnTargetMovement(MovementChainAsset);

        float TotalScore = MovementChainAsset->ScoreBias + DistanceScore + TargetMovementScore;

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestMovementChainDistanceScore = DistanceScore;
            BestMovementChainTargetMovementScore = TargetMovementScore;
            BestMovementChainDataAsset = MovementChainAsset;
        }
    }

    if (GEngine && EnableSelectedDebug)
    {
        GEngine->AddOnScreenDebugMessage(10, 3.5f, FColor::Cyan,
            FString::Printf(TEXT(">> Selected MovementChain: %s | DistanceScore: %.1f | TargetMovementScore: %.1f "),
                *BestMovementChainDataAsset->MovementChainName.ToString(), BestMovementChainDistanceScore, BestMovementChainTargetMovementScore));
    }

    return BestMovementChainDataAsset->MovementChain;
}

TArray<UMovementChainAsset*> UAC_BehaviorDecision::GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const
{
    TArray<UMovementChainAsset*> Result;

    if (!AttackAbilityMovementChainMapAsset || !SelectedAbilityClass)
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

float UAC_BehaviorDecision::CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset)
{
    float Score = 0.0f;

    /*
    // If the distance is shorter than the attack's minimum range, moving backward helps to increase distance and reach effective range.
    if ((DistanceToTarget < SelectedAttackAbilityData.MinRange) && (MovementData.Direction == EMovementDirection::Backward))
    {
        Score += 1.0f;
    }
    */
    /*
    if (MovementData.DistanceScoreCurve)
    {
        float CurveScore = MovementData.DistanceScoreCurve->GetFloatValue(DistanceToTarget);
        Score += CurveScore;
    }
    */
    return Score;
}

float UAC_BehaviorDecision::CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset)
{
    float Score = 0.0f;
    /*
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
    */
    return Score;
}

float UAC_BehaviorDecision::CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset)
{
    return 0.0f;
}

/*
float UAC_BehaviorDecision::CalculateMovementChainScoreBasedOnLastSelectedMovement(FMovementData MovementData, FMovementData LastMovementData)
{
        return 0.0f;
        /*

    if (LastMovementData.MovementName == NAME_None)
    {
        return 0.0f;
    }

    float ChainScore = 0.0f;
    if (MovementData.MovementChainDataBasedOnLastMovementData.LastMovementDirectionScoreModifiers.Contains(LastMovementData.Direction))
    {
        ChainScore += MovementData.MovementChainDataBasedOnLastMovementData.LastMovementDirectionScoreModifiers[LastMovementData.Direction];
    }

    if (MovementData.MovementChainDataBasedOnLastMovementData.LastMovementTypeScoreModifiers.Contains(LastMovementData.MovementType))
    {
        ChainScore += MovementData.MovementChainDataBasedOnLastMovementData.LastMovementTypeScoreModifiers[LastMovementData.MovementType];
    }

    // Weight multiplier
    ChainScore *= MovementData.MovementChainDataBasedOnLastMovementData.ChainScoreWeight;
}
*/