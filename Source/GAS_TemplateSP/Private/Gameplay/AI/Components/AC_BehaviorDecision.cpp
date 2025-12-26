// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/BehaviorDecision/Services/GetBestAttack/BDS_GetBestAttack.h"
#include "Gameplay/AI/BehaviorDecision/Services/BDS_GetBestMovementChain.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/Components/AC_IntendManager.h"

UAC_BehaviorDecision::UAC_BehaviorDecision()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_BehaviorDecision::BeginPlay()
{
	Super::BeginPlay();
}

void UAC_BehaviorDecision::OnHeroSpawned(const FHeroSpawnData& HeroSpawnData)
{
	Super::OnHeroSpawned(HeroSpawnData);

    checkf(OwnerController, TEXT("OwnerController is null in %s"), *GetClass()->GetName());

    IntendManager = OwnerController->GetIntendManagerComponent();
    checkf(IntendManager, TEXT("IntendManager is null in %s"), *GetClass()->GetName());

    HeroMovementListenerComp = HeroBase->GetMovementListenerComponent();
    checkf(HeroMovementListenerComp, TEXT("HeroMovementListenerComp is null in %s"), *GetClass()->GetName());

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
        UE_LOG(LogTemp, Warning, TEXT("Decision: Componnet: BehaviorDecisionConfigAsset is null in: %s !"), *GetName());
        return;
    }

    FBehaviorServiceInitParams ServiceInitData = FBehaviorServiceInitParams(OwnerEnemyBase, OwnerController, OwnerEnemyASC, HeroBase, HeroMovementListenerComp, IntendManager);

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

UAttackDataBase* UAC_BehaviorDecision::GetBestAttack()
{
    if (!GetBestAttackService) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision: Componnet: GetBestAttackService is null in: %s"), *GetName());
        return nullptr;
    }

    UAttackDataBase* BestAttack = GetBestAttackService->GetBestAttack();
    if (!BestAttack) 
    {
        return nullptr;
    }

    LastSelectedAttackData = BestAttack;
    return BestAttack;
}

UMovementChainData* UAC_BehaviorDecision::GetBestMovementChain(UAttackDataBase* SelectedAttackData)
{
    if (!GetBestMovementChainService)
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision: Componnet: GetBestMovementChainService is null in: %s"), *GetName());
        return nullptr;
    }

    UMovementChainData* BestMovementChainData = nullptr;

    if (IsValid(GetBestMovementChainService)) 
    {
        BestMovementChainData = GetBestMovementChainService->GetBestMovementChain(SelectedAttackData);
    }

    if (!BestMovementChainData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision: Componnet: BestMovementChainData is null in: %s"), *GetName());
        return nullptr;
    }

    return BestMovementChainData;
}

UComingAttackReactionData* UAC_BehaviorDecision::GetBestComingAttackReaction(FComingAttackPayload ComingAttackPayload)
{
    if (!IsValid(ComingAttackReactionService) || !ComingAttackPayload.ComingAttack)
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision: Componnet: ComingAttackReactionService or ComingAttack is null in: %s"), *GetName());
        return nullptr;
    }

    UComingAttackReactionData* BestComingAttackData = nullptr;
    BestComingAttackData = ComingAttackReactionService->GetBestComingAttackReaction(ComingAttackPayload);
    return BestComingAttackData;
}



