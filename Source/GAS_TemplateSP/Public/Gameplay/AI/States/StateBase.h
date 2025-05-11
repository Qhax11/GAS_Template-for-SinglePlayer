// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "StateBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter(AGAS_EnemyBase* OwnerEnemy) {}

	virtual void OnTick(AGAS_EnemyBase* OwnerEnemy, float DeltaTime) {}

	virtual void OnExit(AGAS_EnemyBase* OwnerEnemy) {}

};
