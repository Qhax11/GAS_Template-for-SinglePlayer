// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "MovementState.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UMovementState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual void OnEnter() override;

	void StartMovementChain();

	UFUNCTION()
	void OnMovementChainEnded();

private:
	class UAC_EnemyMovementManager* MovementManagerComponent;
};
