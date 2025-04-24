// Qhax's GAS Template for 2D SinglePlayer


#include "BaseClasses/GAS_PlayerController.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"

void AGAS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnPlayerControllerSpawn.Broadcast(this);
    }
}
