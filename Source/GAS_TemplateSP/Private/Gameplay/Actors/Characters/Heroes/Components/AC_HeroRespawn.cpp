// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroRespawn.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "GameFramework/GameMode.h"


void UAC_HeroRespawn::BindCharacterDeSpawn()
{
    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnHeroDeSpawn.AddDynamic(this, &UAC_HeroRespawn::OnHeroDeSpawn);
    }
}

void UAC_HeroRespawn::OnHeroDeSpawn(const FCharacterDeSpawnData& HeroDeSpawnData)
{
    if (HeroDeSpawnData.Character != OwnerCharacter)
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(CharacterDeSpawnCountDownTimerHandle, [this, HeroDeSpawnData]()
        {
            FHeroSpawnData HeroSpawnData = FHeroSpawnData(HeroDeSpawnData.Character, HeroDeSpawnData.ASC);
            OnHeroReSpawn(HeroSpawnData);
        },
        ReSpawnDelay, false);
}

void UAC_HeroRespawn::OnHeroReSpawn(const FHeroSpawnData& HeroReSpawnData)
{
    SetHeroLocation(HeroReSpawnData.Character);

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnHeroReSpawn.Broadcast(HeroReSpawnData);
    }
}

void UAC_HeroRespawn::SetHeroLocation(AGAS_CharacterBase* Hero)
{
    if (!Hero)
    {
        return;
    }

    if (UWorld* World = this->GetWorld())
    {
        if (AGameModeBase* AuthGameMode = World->GetAuthGameMode())
        {
            if (AActor* StartPoint = AuthGameMode->FindPlayerStart(Hero->GetController()))
            {
                Hero->SetActorLocation(StartPoint->GetActorLocation());
                Hero->SetActorRotation(StartPoint->GetActorRotation());
            }
        }
    }
}



