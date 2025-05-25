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

	virtual void OnTick(float DeltaTime) override;

	virtual void OnExit() override;

	void TryEnterToAttackState();

	bool IsInRange();

	void StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackAbilityClass);

	UFUNCTION()
	void OnMovementChainEnded();

	FAttackData BestAttack;
	UGAS_GameplayAbilityBase* SelectedAttackCDO;

private:
	class UAC_EnemyMovementManager* MovementManagerComponent;
};
