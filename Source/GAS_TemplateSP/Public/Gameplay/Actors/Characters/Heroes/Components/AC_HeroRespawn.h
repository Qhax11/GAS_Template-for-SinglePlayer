// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_RespawnBase.h"
#include "AC_HeroRespawn.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroRespawn : public UAC_RespawnBase
{
	GENERATED_BODY()

protected:
	virtual void BindCharacterDeSpawn() override;

	UFUNCTION()
	virtual void OnHeroDeSpawn(const FCharacterDeSpawnData& HeroDeSpawnData);

	virtual void OnHeroReSpawn(const FHeroSpawnData& HeroReSpawnData);

	void SetHeroLocation(AGAS_CharacterBase* Hero);
};
