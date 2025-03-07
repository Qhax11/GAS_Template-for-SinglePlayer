// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Death/GA_HeroDeathBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"


void UGA_HeroDeathBase::BroadcastDeSpawn()
{
	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		SpawnDelegatesSubsystem->OnHeroDeSpawn.Broadcast(Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()));
	}
}


