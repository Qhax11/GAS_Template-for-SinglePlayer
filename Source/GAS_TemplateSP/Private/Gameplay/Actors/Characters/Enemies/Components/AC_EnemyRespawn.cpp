// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyRespawn.h"
#include "AIController.h"
#include "BrainComponent.h"

void UAC_EnemyRespawn::BindCharacterDeSpawn()
{
    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnEnemyDeSpawn.AddDynamic(this, &UAC_RespawnBase::StartCharacterReSpawnCountdown);
    }
}

void UAC_EnemyRespawn::OnCharacterRespawn(const FCharacterSpawnData& EnemySpawnData)
{
    Super::OnCharacterRespawn(EnemySpawnData);

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnEnemyReSpawn.Broadcast(EnemySpawnData);
    }

    AAIController* EnemyController = Cast<AAIController>(EnemySpawnData.CharacterBase->GetController());
    if (!EnemyController)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyController is null in: %s"), *GetName());
        return;
    }

    EnemyController->GetBrainComponent()->StartLogic();
}



