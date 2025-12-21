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

void UAC_IntendManager::OnMovementChainCompleted(const FMovementChainEndData& MovementChainEndData)
{ 
    UMovementChainData* EndedChainData = MovementChainEndData.ChainData;
    if (!EndedChainData)
    {
        return;
    }

    if (EndedChainData->FallbackPolicy == EMovementChainFallbackPolicy::HardFallback)
    {
        EscalateIntent();
    }
}

void UAC_IntendManager::OnMovementChainAborted(const UMovementChainData* Chain)
{
    DeescalateIntent();
}

void UAC_IntendManager::EscalateIntent()
{
    if (CurrentIntent == EAIIntent::LowPressure) 
    {
        CurrentIntent = EAIIntent::MidPressure;
    }
    else if (CurrentIntent == EAIIntent::MidPressure)
    {
        CurrentIntent = EAIIntent::HighPressure;
    }
}

void UAC_IntendManager::DeescalateIntent()
{
    if (CurrentIntent == EAIIntent::HighPressure)
    {
        CurrentIntent = EAIIntent::MidPressure;
    }
}
