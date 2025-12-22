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

    AAIControllerBase* OwnerController = Cast<AAIControllerBase>(GetOwner());
    checkf(OwnerController, TEXT("OwnerController is null in %s"), *GetClass()->GetName());

    AGAS_EnemyBase* OwnerEnemyBase = Cast<AGAS_EnemyBase>(OwnerController->GetPawn());
    checkf(OwnerEnemyBase, TEXT("OwnerEnemyBase is null in %s"), *GetClass()->GetName());

    MovementManager = OwnerEnemyBase->GetEnemyMovementManagerComponent();
    checkf(MovementManager, TEXT("MovementManager is null in %s"), *GetClass()->GetName());

    MeleeComboManager = OwnerEnemyBase->GetEnemyMeleeComboManagerComponent();
    checkf(MeleeComboManager, TEXT("MeleeComboManager is null in %s"), *GetClass()->GetName());

    // Bind delegate
    MovementManager->OnMovementChainEnded.RemoveAll(this);
    MovementManager->OnMovementChainEnded.AddUObject(this, &UAC_IntendManager::OnMovementChainEnd);
	MeleeComboManager->OnEnemyComboChainEnded.RemoveAll(this);
	MeleeComboManager->OnEnemyComboChainEnded.AddUObject(this, &UAC_IntendManager::OnEnemyComboChainEnd);
}

void UAC_IntendManager::OnMovementChainEnd(const FMovementChainEndData& EndData)
{ 
    if (!EndData.ChainData)
    {
        return;
    }
    
    if (EndData.Result == EMovementChainResult::Completed)
    {
        HandleMovementChainCompleted(EndData);
    }
    else if (EndData.Result == EMovementChainResult::Aborted)
    {
        DecreasePressure();
        return;
    }
}

void UAC_IntendManager::OnEnemyComboChainEnd(const FEnemyComboChainEndData& EndData)
{
    if (EndData.Result == EEnemyComboChainResult::Completed)
    {
        HandleComboChainCompleted(EndData);
    }
    else if (EndData.Result == EEnemyComboChainResult::OutOfRange)
    {
        return;
    }
    else if (EndData.Result == EEnemyComboChainResult::HitTaken)
    {
        DecreasePressure();
    }
    else if (EndData.Result == EEnemyComboChainResult::ParryTriggered)
    {
        IncreasePressure();
    }
    else if (EndData.Result == EEnemyComboChainResult::DodgeTriggered)
    {
        return;
    }
}

void UAC_IntendManager::HandleMovementChainCompleted(const FMovementChainEndData& EndData)
{
    if (EndData.ChainData->FallbackPolicy == EMovementChainFallbackPolicy::SoftFallback)
    {
        ConsecutiveSoftMovementCount++;
        if (ConsecutiveSoftMovementCount >= 2)
        {
            IncreasePressure();
            ConsecutiveSoftMovementCount = 0;
        }
        return;
    }

    if (EndData.ChainData->FallbackPolicy == EMovementChainFallbackPolicy::HardFallback)
    {
        ConsecutiveSoftMovementCount = 0;
        IncreasePressure();
    }
}

void UAC_IntendManager::HandleComboChainCompleted(const FEnemyComboChainEndData& EndData)
{
    IncreasePressure();
}

void UAC_IntendManager::IncreasePressure()
{
    UE_LOG(LogTemp, Log, TEXT("INTEND SET | this=%p | Before=%d"), this, (int32)CurrentIntent);
    if (CurrentIntent == EEnemyIntent::LowPressure)
    {
        CurrentIntent = EEnemyIntent::MidPressure;
    }
    else if (CurrentIntent == EEnemyIntent::MidPressure)
    {
        CurrentIntent = EEnemyIntent::HighPressure;
    }
    UE_LOG(LogTemp, Log, TEXT("INTEND SET | this=%p | After=%d"), this, (int32)CurrentIntent);
}

void UAC_IntendManager::DecreasePressure()
{
    if (CurrentIntent == EEnemyIntent::HighPressure)
    {
        CurrentIntent = EEnemyIntent::MidPressure;
    }
}

