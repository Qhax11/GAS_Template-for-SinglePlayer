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

	virtual void OnEnter_Implementation() override;

	virtual void OnExit_Implementation() override;

	virtual void OnTick_Implementation(float DeltaTime) override;

	void TryEnterToAttackState();

	void StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackAbilityClass);

	UFUNCTION()
	void OnMovementChainEnded();

	FAttackData SelectedAttack;
	UGAS_GameplayAbilityBase* SelectedAttackCDO;

private:
	class UAC_EnemyMovementManager* MovementManagerComponent;
};
