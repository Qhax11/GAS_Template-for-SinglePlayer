// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "StateBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter(AGAS_EnemyBase* OwnerEnemy, AAIControllerBase* OwnerController) {}

	virtual void OnTick(AGAS_EnemyBase* OwnerEnemy, AAIControllerBase* OwnerController, float DeltaTime) {}

	virtual void OnExit(AGAS_EnemyBase* OwnerEnemy, AAIControllerBase* OwnerController) {}
};
