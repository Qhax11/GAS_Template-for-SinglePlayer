// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyRespawn.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

void UAC_EnemyRespawn::BeginPlay()
{
    Super::BeginPlay();

    EnemyController = Cast<AAIControllerBase>(OwnerCharacter->GetController());
    if (!EnemyController)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyController is null in: %s"), *GetName());
        return;
    }
}

void UAC_EnemyRespawn::BindCharacterDeSpawn()
{
    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnEnemyDeSpawn.AddDynamic(this, &UAC_EnemyRespawn::OnEnemyDeSpawn);
    }
}

void UAC_EnemyRespawn::OnEnemyDeSpawn(const FCharacterDeSpawnData& EnemyDeSpawnData)
{
    if (EnemyDeSpawnData.Character != OwnerCharacter || EnemyDeSpawnData.DeSpawnPhase == EDeSpawnPhase::DeathStarted)
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(CharacterDeSpawnCountDownTimerHandle, [this, EnemyDeSpawnData]()
        {
            FEnemySpawnData CharacterSpawnData = FEnemySpawnData(EnemyDeSpawnData.Character, EnemyDeSpawnData.ASC);
            OnEnemyReSpawn(CharacterSpawnData);
        },
        ReSpawnDelay, false);
}

void UAC_EnemyRespawn::OnEnemyReSpawn(const FEnemySpawnData& EnemySpawnData)
{
    if (bDisableSpawn)
    {
        return;
    }

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnEnemyReSpawn.Broadcast(EnemySpawnData);
    }

    EnemySpawnData.Character->EnableMovement();
    EnemySpawnData.Character->EnableCollision();
    EnemySpawnData.Character->EnableMesh();
    ApplyCharacterReSpawnEffect(EnemySpawnData.Character);
    OnCharacterReSpawn.Broadcast(EnemySpawnData.Character);
}

void UAC_EnemyRespawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnEnemyDeSpawn.RemoveDynamic(this, &UAC_EnemyRespawn::OnEnemyDeSpawn);
    }
}
