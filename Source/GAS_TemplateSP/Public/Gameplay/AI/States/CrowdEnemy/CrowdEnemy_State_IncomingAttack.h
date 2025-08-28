// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/InComingAttackState.h"
#include "CrowdEnemy_State_IncomingAttack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UCrowdEnemy_State_IncomingAttack : public UInComingAttackState
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter_Implementation() override;

	virtual void OnExit_Implementation() override;

};
