// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/StateTree/ST_Base.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include <Kismet/GameplayStatics.h>

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

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    HeroBase = Cast<AGAS_HeroBase>(PlayerCharacter);
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

    InitalizeServiceses();

   // OwnerController->OnTargetDetected.AddDynamic(this, &UAC_BehaviorDecision::OnTargetDetected);
}

void UAC_BehaviorDecision::OnTargetDetected(AActor* Target)
{
    InitalizeServiceses();
}

void UAC_BehaviorDecision::InitalizeServiceses()
{
    for (UBDS_ComingAttackReactionBase* ReactionInstance : ComingAttackReactionAsset->ComingAttackReactions)
    {
        if (ReactionInstance) 
        {
            FBehaviorServiceInitParams ComingAttackReactionServiceInitData = FBehaviorServiceInitParams(
                ComingAttackReactionAsset, OwnerEnemyBase, OwnerController, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
            ReactionInstance->Initialize(ComingAttackReactionServiceInitData);
        }
    }

    GetBestAttackService = NewObject<UBDS_GetBestAttack>(GetOwner());
    FBehaviorServiceInitParams GetBestAttackServiceInitData = FBehaviorServiceInitParams
    (AttackAbilityAsset, OwnerEnemyBase, OwnerController, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
    GetBestAttackService->Initialize(GetBestAttackServiceInitData);

    GetBestMovementChainService = NewObject<UBDS_GetBestMovementChain>(GetOwner());
    FBehaviorServiceInitParams GetBestMovementChainServiceInitData = FBehaviorServiceInitParams(
        AttackAbilityMovementChainMapAsset, OwnerEnemyBase, OwnerController, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
    GetBestMovementChainService->Initialize(GetBestMovementChainServiceInitData);
}

FAttackData UAC_BehaviorDecision::GetBestAttack()
{
    if (!GetBestAttackService) 
    {
        UE_LOG(LogTemp, Warning, TEXT("GetBestAttackService is null in: %s"), *GetName());
        return FAttackData();
    }

    FAttackData BestAttack = GetBestAttackService->GetBestAttack();

#if WITH_EDITOR
    if (GEngine && EnableSelectedDebug && BestAttack.AbilityClass)
    {
        GEngine->AddOnScreenDebugMessage(9, 3.5f, FColor::Red,
            FString::Printf(TEXT(">> Selected Attack: %s"),
                *BestAttack.AbilityClass->GetName()));
    }
#endif // WITH_EDITOR

    LastSelectedAttackAbilityData = BestAttack;
    return BestAttack;
}

TArray<FMovementAbilityData> UAC_BehaviorDecision::GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    if (!GetBestMovementChainService)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetBestMovementChainService is null in: %s"), *GetName());
        return TArray<FMovementAbilityData>();
    }

    UMovementChainAsset* BestMovementChainDataAsset = nullptr;

    if (IsValid(GetBestMovementChainService)) 
    {
        BestMovementChainDataAsset = GetBestMovementChainService->GetBestMovementChain(SelectedAbilityClass);
    }

#if WITH_EDITOR
    if (GEngine && EnableSelectedDebug)
    {
        FString ChainNameStr = BestMovementChainDataAsset
            ? BestMovementChainDataAsset->MovementChainName.ToString()
            : TEXT("INVALID");

        GEngine->AddOnScreenDebugMessage(10, 3.5f, FColor::Cyan,
            FString::Printf(TEXT(">> Selected MovementChain: %s"), *ChainNameStr));
    }
#endif // WITH_EDITOR

    if (!BestMovementChainDataAsset) 
    {
        UE_LOG(LogTemp, Warning, TEXT("BestMovementChainDataAsset is null in: %s"), *GetName());
        return TArray<FMovementAbilityData>();
    }

    return BestMovementChainDataAsset->MovementChain;
}

UBDS_ComingAttackReactionBase* UAC_BehaviorDecision::GetBestComingAttackReaction(FComingAttackPayload ComingAttackPayload)
{
    UBDS_ComingAttackReactionBase* BestComingAttackInstance = nullptr;

    if (!IsValid(ComingAttackReactionAsset) || !ComingAttackPayload.ComingAttack)
    {
        return BestComingAttackInstance;
    }

    EComingAttackReaction BestReaction = EComingAttackReaction::TakeDamage;
    float BestScore = -FLT_MAX;

    for (UBDS_ComingAttackReactionBase* ReactionInstance : ComingAttackReactionAsset->ComingAttackReactions)
    {
        if (!ReactionInstance) 
        {
            UE_LOG(LogTemp, Warning, TEXT("ReactionInstance is null in: %s"), *GetName());
            continue;
        }

        if (!ReactionInstance->IsEnable(ComingAttackPayload))
        {
            continue;
        }

        float ComingAttackReactionScore = ReactionInstance->CalculateComingAttackReactionScore(ComingAttackPayload);
        UE_LOG(LogTemp, Log, TEXT("[AI] Reaction %s → Score: %.2f"), *ReactionInstance->ComingAttackReactionName.ToString(), ComingAttackReactionScore);

        if (ComingAttackReactionScore > BestScore)
        {
            BestScore = ComingAttackReactionScore;
            BestComingAttackInstance = ReactionInstance;
        }
    }

    if (BestComingAttackInstance)
    {
        BestComingAttackInstance->InitializeAfterSelection();
        LastSelectedComingAttackReaction = BestComingAttackInstance;
        UE_LOG(LogTemp, Log, TEXT("[AI] SelectedReaction %s"), *BestComingAttackInstance->ComingAttackReactionName.ToString());
    }

    return BestComingAttackInstance;
}

