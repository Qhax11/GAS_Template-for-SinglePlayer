// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"
#include "Gameplay/AI/BehaviorDecision/Services/GetBestAttack/BDS_GetBestAttack.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/AI/BehaviorDecision/Services/BDS_GetBestMovementChain.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"


UAC_BehaviorDecision::UAC_BehaviorDecision()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_BehaviorDecision::BeginPlay()
{
	Super::BeginPlay();

    if(!OwnerController || !OwnerEnemyBase || !OwnerEnemyASC || !HeroBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("Some owner variables are already set in: %s !"), *GetName());
        return;
	}

    HeroMovementListenerComp = HeroBase->GetMovementListenerComponent();
    if (!HeroMovementListenerComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroMovementListenerComp is null in: %s !"), *GetName());
        return;
    }

    OwnerController->OnTargetDetected.AddDynamic(this, &UAC_BehaviorDecision::OnTargetDetected);
}

void UAC_BehaviorDecision::OnTargetDetected(AActor* DetectedTarget)
{
    CreateAndInitalizeServiceses();
}

void UAC_BehaviorDecision::CreateAndInitalizeServiceses()
{
    if (!BehaviorDecisionConfigAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionConfigAsset is null in: %s !"), *GetName());
        return;
    }

    FBehaviorServiceInitParams ServiceInitData = FBehaviorServiceInitParams(OwnerEnemyBase, OwnerController, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, BehaviorState);

    if (BehaviorDecisionConfigAsset->AttackDecisionServiceClass) 
    {
        GetBestAttackService = NewObject<UBDS_GetBestAttack>(this, BehaviorDecisionConfigAsset->AttackDecisionServiceClass);
        GetBestAttackService->Initialize(ServiceInitData);
    }

    if (BehaviorDecisionConfigAsset->MovementChainServiceClass)
    {
        GetBestMovementChainService = NewObject<UBDS_GetBestMovementChain>(this, BehaviorDecisionConfigAsset->MovementChainServiceClass);
        GetBestMovementChainService->Initialize(ServiceInitData);
    }

    if (BehaviorDecisionConfigAsset->ComingAttackReactionServiceClass)
    {
        ComingAttackReactionService = NewObject<UBDS_ComingAttackReactionBase>(this, BehaviorDecisionConfigAsset->ComingAttackReactionServiceClass);
        ComingAttackReactionService->Initialize(ServiceInitData);
    }

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

UComingAttackReactionData* UAC_BehaviorDecision::GetBestComingAttackReaction(FComingAttackPayload ComingAttackPayload)
{
    if (!IsValid(ComingAttackReactionService) || !ComingAttackPayload.ComingAttack)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetBestAttackService is null in: %s"), *GetName());
        return nullptr;
    }

    UComingAttackReactionData* BestComingAttackData = nullptr;
    BestComingAttackData = ComingAttackReactionService->GetBestComingAttackReaction(ComingAttackPayload);
    return BestComingAttackData;
}

