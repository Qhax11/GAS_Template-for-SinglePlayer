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

        bool bIsOnCooldown = OwnerEnemyASC->HasMatchingGameplayTag(Attack.AbilityCooldownTag);
        if (bIsOnCooldown) 
        {
            continue;
        }

        float DistanceScore = CalculateAttackAbilityScoreBasedOnTargetDistance(DistanceToTarget, Attack.MinRange, Attack.MaxRange);

        float TotalScore = Attack.ScoreBias + DistanceScore;

        UE_LOG(LogTemp, Log, TEXT("[AI] Attack %s → Score: %.2f"), *Attack.AbilityClass->GetName(), TotalScore);

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestAttack = Attack;
        }
    }

    if (GEngine && EnableSelectedDebug)
    {
        GEngine->AddOnScreenDebugMessage(9, 3.5f, FColor::Red,
            FString::Printf(TEXT(">> Selected Attack: %s | DistanceScore: %.1f"),
                *BestAttack.AbilityClass->GetName(), BestAttackDistanceScore));
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
    if (AbilityMovementChainAssets.IsEmpty()) 
    {
        return TArray<FMovementAbilityData>();
    }

    for (UMovementChainAsset* MovementChainAsset : AbilityMovementChainAssets)
    {
        if (GetTargetDistance() < MovementChainAsset->MinRange)
        {
            continue;
        }

        float DistanceScore = CalculateMovementChainScoreBasedOnTargetDistance(MovementChainAsset);
        float TargetMovementScore = CalculateMovementChainScoreBasedOnTargetMovement(MovementChainAsset);

        float TotalScore = MovementChainAsset->ScoreBias + DistanceScore + TargetMovementScore;

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

    if (MovementChainAsset->DistanceScoreCurve)
    {
        float CurveScore = MovementChainAsset->DistanceScoreCurve->GetFloatValue(GetTargetDistance());
        Score += CurveScore;
    }
    
    return Score;
}

float UAC_BehaviorDecision::CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset)
{
    return 0.0f;
}

float UAC_BehaviorDecision::CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset)
{
    return 0.0f;
}

bool UAC_BehaviorDecision::ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset)
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

FGameplayTag UAC_BehaviorDecision::GetRandomDirectionTag()
{
    static const TArray<FGameplayTag> PossibleDirections =
    {
        GAS_Tags::TAG_AI_Direction_Resolved_Forward,
        GAS_Tags::TAG_AI_Direction_Resolved_Backward,
        GAS_Tags::TAG_AI_Direction_Resolved_Left,
        GAS_Tags::TAG_AI_Direction_Resolved_Right     
    };

    int32 RandomIndex = FMath::RandRange(0, PossibleDirections.Num() - 1);
    return PossibleDirections[RandomIndex];
}

float UAC_BehaviorDecision::GetTargetDistance()
{
    if (!OwnerEnemyBase || !OwnerController || !OwnerController->GetTarget())
    {
        return -1.f;
    }

    FVector MyLocation = OwnerEnemyBase->GetActorLocation();
    FVector TargetLocation = OwnerController->GetTarget()->GetActorLocation();

    return FVector::Dist(MyLocation, TargetLocation);
}

