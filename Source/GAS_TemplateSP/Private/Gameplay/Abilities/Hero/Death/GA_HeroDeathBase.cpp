// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Death/GA_HeroDeathBase.h"

void UGA_HeroDeathBase::BroadcastDeSpawn()
{
	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		FCharacterSpawnData CharacterSpawnData = FCharacterSpawnData(
			Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()), GetAbilitySystemComponentFromActorInfo());
		SpawnDelegatesSubsystem->OnHeroDeSpawn.Broadcast(CharacterSpawnData);
	}
}
