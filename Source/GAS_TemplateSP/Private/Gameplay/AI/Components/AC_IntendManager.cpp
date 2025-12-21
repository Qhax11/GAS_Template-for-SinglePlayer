// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_IntendManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainData.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"

void UAC_IntendManager::BeginPlay()
{
    Super::BeginPlay();

    if (!MovementManager)
    {
        return;
    }

    // Bind end deleagte
    OwnerEnemyBase->GetEnemyMovementManagerComponent();
    MovementManager->OnMovementChainEnded.RemoveAll(this);
    MovementManager->OnMovementChainEnded.AddUObject(this, &UAC_IntendManager::OnMovementChainCompleted);
}

void UAC_IntendManager::OnMovementChainCompleted(const FMovementChainEndData& EndData)
{ 
    if (!EndData.ChainData)
    {
        return;
    }

    // Abort → her zaman geri çekil
    if (EndData.Result == EMovementChainResult::Aborted)
    {
        DecreasePressure();
        return;
    }

    // Completed ama soft → intent sabit
    if (EndData.ChainData->FallbackPolicy == EMovementChainFallbackPolicy::SoftFallback)
    {
        return;
    }

    // Completed + HardFallback → intent değişebilir
    if (EndData.Result == EMovementChainResult::Completed && EndData.ChainData->FallbackPolicy == EMovementChainFallbackPolicy::HardFallback)
    {
        IncreasePressure();
    }
}

void UAC_IntendManager::IncreasePressure()
{
    if (CurrentIntent == EEnemyIntent::LowPressure)
    {
        CurrentIntent = EEnemyIntent::MidPressure;
    }
    else if (CurrentIntent == EEnemyIntent::MidPressure)
    {
        CurrentIntent = EEnemyIntent::HighPressure;
    }
}

void UAC_IntendManager::DecreasePressure()
{
    if (CurrentIntent == EEnemyIntent::HighPressure)
    {
        CurrentIntent = EEnemyIntent::MidPressure;
    }
}

