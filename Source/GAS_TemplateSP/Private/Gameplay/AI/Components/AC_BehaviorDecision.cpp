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

    InitalizeServiceses();
}

void UAC_BehaviorDecision::InitalizeServiceses()
{
    ComingAttackReactionService = NewObject<UBDS_ComingAttackReaction>(GetOwner());
    FBehaviorServiceInitParams ComingAttackReactionServiceInitData = FBehaviorServiceInitParams(
        ComingAttackReactionAsset, OwnerEnemyBase, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
    ComingAttackReactionService->Initialize(ComingAttackReactionServiceInitData);

    GetBestAttackService = NewObject<UBDS_GetBestAttack>(GetOwner());
    FBehaviorServiceInitParams GetBestAttackServiceInitData = FBehaviorServiceInitParams
    (AttackAbilityAsset, OwnerEnemyBase, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
    GetBestAttackService->Initialize(GetBestAttackServiceInitData);

    GetBestMovementChainService = NewObject<UBDS_GetBestMovementChain>(GetOwner());
    FBehaviorServiceInitParams GetBestMovementChainServiceInitData = FBehaviorServiceInitParams(
        AttackAbilityMovementChainMapAsset, OwnerEnemyBase, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);
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

    if (GEngine && EnableSelectedDebug)
    {
        FString ChainNameStr = BestMovementChainDataAsset
            ? BestMovementChainDataAsset->MovementChainName.ToString()
            : TEXT("INVALID");

        GEngine->AddOnScreenDebugMessage(10, 3.5f, FColor::Cyan,
            FString::Printf(TEXT(">> Selected MovementChain: %s"), *ChainNameStr));
    }

    return BestMovementChainDataAsset->MovementChain;
}

FComingAttackReactionData UAC_BehaviorDecision::GetBestComingAttackDecision(FComingAttackPayload ComingAttackPayload)
{
    FComingAttackReactionData BestComingAttackReaction;
    if (!ComingAttackReactionService)
    {
        UE_LOG(LogTemp, Warning, TEXT("ComingAttackReactionService is null in: %s"), *GetName());
        return BestComingAttackReaction;
    }

    if (IsValid(ComingAttackReactionService)) 
    {
        BestComingAttackReaction = ComingAttackReactionService->GetBestComingAttackDecision(ComingAttackPayload);
    }

    return BestComingAttackReaction;
}

