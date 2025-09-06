// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_RespawnBase.h"
#include "AC_EnemyRespawn.generated.h"

class AAIControllerBase;
class UST_Base;

UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyRespawn : public UAC_RespawnBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void BindCharacterDeSpawn() override;

	UFUNCTION()
	virtual void OnEnemyDeSpawn(const FCharacterDeSpawnData& EnemySpawnData);

	virtual void OnEnemyReSpawn(const FEnemySpawnData& EnemySpawnData);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	AAIControllerBase* EnemyController;
	UST_Base* StateTree;

};
