// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "CrowdEnemy_State_Attack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UCrowdEnemy_State_Attack : public UStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter_Implementation() override;

	virtual void OnExit_Implementation() override;

	UFUNCTION()
	void OnComboChaindEnded();
};
