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

    ComingAttackReactionService = NewObject<UBDS_ComingAttackReaction>(this);
    FBehaviorServiceInitParams ComingAttackReactionServiceInitData = FBehaviorServiceInitParams(
        ComingAttackReactionAsset, OwnerEnemyBase, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
    ComingAttackReactionService->Initialize(ComingAttackReactionServiceInitData);

    GetBestAttackService = NewObject<UBDS_GetBestAttack>(this);
    FBehaviorServiceInitParams GetBestAttackServiceInitData = FBehaviorServiceInitParams
    (AttackAbilityAsset, OwnerEnemyBase, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
    GetBestAttackService->Initialize(GetBestAttackServiceInitData);

    GetBestMovementChainService = NewObject<UBDS_GetBestMovementChain>(this);
    FBehaviorServiceInitParams GetBestMovementChainServiceInitData = FBehaviorServiceInitParams(
        AttackAbilityMovementChainMapAsset, OwnerEnemyBase, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
    GetBestMovementChainService->Initialize(GetBestMovementChainServiceInitData);
}

FAttackData UAC_BehaviorDecision::GetBestAttack(float DistanceToTarget)
{
    FAttackData BestAttack = GetBestAttackService->GetBestAttack(DistanceToTarget);

    if (GEngine && EnableSelectedDebug && BestAttack.AbilityClass)
    {
        GEngine->AddOnScreenDebugMessage(9, 3.5f, FColor::Red,
            FString::Printf(TEXT(">> Selected Attack: %s"),
                *BestAttack.AbilityClass->GetName()));
    }

    LastSelectedAttackAbilityData = BestAttack;
    return BestAttack;
}

TArray<FMovementAbilityData> UAC_BehaviorDecision::GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    return GetBestMovementChainService->GetBestMovementChain(SelectedAbilityClass);

    /*
    if (GEngine && EnableSelectedDebug)
    {
        GEngine->AddOnScreenDebugMessage(10, 3.5f, FColor::Cyan,
            FString::Printf(TEXT(">> Selected MovementChain: %s | DistanceScore: %.1f | TargetMovementScore: %.1f "),
                *BestMovementChainDataAsset->MovementChainName.ToString(), BestMovementChainDistanceScore, BestMovementChainTargetMovementScore));
    }

    return BestMovementChainDataAsset->MovementChain;
    */
}

EComingAttackReaction UAC_BehaviorDecision::GetComingAttackDecision(FComingAttackPayload ComingAttackPayload)
{
    return ComingAttackReactionService->GetComingAttackDecision(ComingAttackPayload);
}

