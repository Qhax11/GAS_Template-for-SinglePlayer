// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "MovementState.generated.h"

class UMovementChainDataa;
class UMovementSingleData;

UCLASS()
class GAS_TEMPLATESP_API UMovementState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void ExecuteMovement(TSharedPtr<FMovementStatePayload> MovementStatePayload);

	void ActivateMovementChain(UMovementChainDataa* MovementChainData);

	void ActivateMovementSingle(UMovementSingleData* MovementSingleData);

	virtual void OnTick_Implementation(float DeltaTime) override;

	void TryEnterToAttackState();

	bool IsInRangeForAttack() const;

	UFUNCTION()
	void OnMovementChainEnded();

	virtual void OnExit_Implementation() override;

protected:
	TSharedPtr<FMovementStatePayload> MovementStateEnterPayload;

	UPROPERTY()
	UGAS_GameplayAbilityBase* SelectedAttackCDO;
};
