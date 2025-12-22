// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_IntendManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainData.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

void UAC_IntendManager::BeginPlay()
{
    Super::BeginPlay();

    check(OwnerEnemyBase);
    check(MovementManager);
    check(MeleeComboManager);

    // Bind delegate
    MovementManager->OnMovementChainEnded.RemoveAll(this);
    MovementManager->OnMovementChainEnded.AddUObject(this, &UAC_IntendManager::OnMovementChainCompleted);
	MeleeComboManager->OnEnemyComboChainEnded.RemoveAll(this);
	MeleeComboManager->OnEnemyComboChainEnded.AddUObject(this, &UAC_IntendManager::OnEnemyComboChainCompleted);
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
        UE_LOG(LogTemp, Log, TEXT("Execution: Movement: UAC_IntendManager: Completed + HardFallback, IncreasePressure."));
        IncreasePressure();
    }
}

void UAC_IntendManager::OnEnemyComboChainCompleted(const FEnemyComboChainEndData& EndData)
{

}

void UAC_IntendManager::IncreasePressure()
{
    UE_LOG(LogTemp, Error, TEXT("INTEND SET | this=%p | Before=%d"), this, (int32)CurrentIntent);

    if (CurrentIntent == EEnemyIntent::LowPressure)
    {
        CurrentIntent = EEnemyIntent::MidPressure;
    }
    else if (CurrentIntent == EEnemyIntent::MidPressure)
    {
        CurrentIntent = EEnemyIntent::HighPressure;
    }

    UE_LOG(LogTemp, Error, TEXT("INTEND SET | this=%p | After=%d"), this, (int32)CurrentIntent);
}

void UAC_IntendManager::DecreasePressure()
{
    if (CurrentIntent == EEnemyIntent::HighPressure)
    {
        CurrentIntent = EEnemyIntent::MidPressure;
    }
}

