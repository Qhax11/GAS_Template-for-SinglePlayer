// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Death/GA_HeroDeathBase.h"

void UGA_HeroDeathBase::BroadcastDeSpawn(const FCharacterDeSpawnData& DespawnData)
{
	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		SpawnDelegatesSubsystem->OnHeroDeSpawn.Broadcast(DespawnData);
	}
}
