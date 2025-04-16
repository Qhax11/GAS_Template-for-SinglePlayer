// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_DeathBase.h"
#include "GA_HeroDeathBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroDeathBase : public UGA_DeathBase
{
	GENERATED_BODY()
	
protected:
	virtual void BroadcastDeSpawn(const FCharacterDeSpawnData& DespawnData) override;

};
