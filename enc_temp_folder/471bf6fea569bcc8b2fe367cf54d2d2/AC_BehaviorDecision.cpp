// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "Gameplay/AI/BehaviorDecision/Services/GetBestAttack/BDS_GetBestAttack.h"
#include "Gameplay/AI/BehaviorDecision/Services/BDS_GetBestMovementChain.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"

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

UAttackDataBase* UAC_BehaviorDecision::GetBestAttack()
{
    if (!GetBestAttackService) 
    {
        UE_LOG(LogTemp, Warning, TEXT("GetBestAttackService is null in: %s"), *GetName());
        return nullptr;
    }

    UAttackDataBase* BestAttack = GetBestAttackService->GetBestAttack();
    if (!BestAttack) 
    {
        return nullptr;
    }

    LastSelectedAttackData = BestAttack;
    if (BestAttack->AbilityClass)
    {
        LastSelectedAttackAbilityCDO = BestAttack->AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
    }

    return BestAttack;
}

UMovementChainDataa* UAC_BehaviorDecision::GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
    if (!GetBestMovementChainService)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetBestMovementChainService is null in: %s"), *GetName());
        return nullptr;
    }

    UMovementChainDataa* BestMovementChainData = nullptr;

    if (IsValid(GetBestMovementChainService)) 
    {
        BestMovementChainData = GetBestMovementChainService->GetBestMovementChain(SelectedAbilityClass);
    }

    if (!BestMovementChainData)
    {
        UE_LOG(LogTemp, Warning, TEXT("BestMovementChainData is null in: %s"), *GetName());
        return nullptr;
    }

    return BestMovementChainData;
}

UComingAttackReactionData* UAC_BehaviorDecision::GetBestComingAttackReaction(FComingAttackPayload ComingAttackPayload)
{
    if (!IsValid(ComingAttackReactionService) || !ComingAttackPayload.ComingAttack)
    {
        UE_LOG(LogTemp, Warning, TEXT("ComingAttackReactionService or ComingAttack is null in: %s"), *GetName());
        return nullptr;
    }

    UComingAttackReactionData* BestComingAttackData = nullptr;
    BestComingAttackData = ComingAttackReactionService->GetBestComingAttackReaction(ComingAttackPayload);
    return BestComingAttackData;
}


float UAC_BehaviorDecision::GetTargetDistance() const
{
    if (!OwnerController)
    {
        return -1.0f;
    }

    return OwnerController->GetTargetHeroDistance();
}

bool UAC_BehaviorDecision::IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass) const
{
    if (!AbilityClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability Class is null in: %s"), *GetName());
        return false;
    }

    UGAS_GameplayAbilityBase* AbilityCDO = AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
    if (!AbilityCDO) 
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityCDO is null in: %s"), *GetName());
        return false;
    }

    float Distance = GetTargetDistance();
    return (AbilityCDO->MaxRange > Distance && AbilityCDO->MinRange < Distance);
}

