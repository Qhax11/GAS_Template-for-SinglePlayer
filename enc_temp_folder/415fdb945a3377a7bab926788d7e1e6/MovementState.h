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

	virtual bool EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload = nullptr) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

protected:
	void StartMovementChain(UMovementChainDataa* MovementChainData);

public:
	virtual void OnTick_Implementation(float DeltaTime) override;

	void TryEnterToAttackState();

	bool IsReachedAttackRange() const;

	UFUNCTION()
	void OnMovementChainEnded();

	virtual void OnExit_Implementation() override;

protected:
	TSharedPtr<FMovementStatePayload> MovementStateEnterPayload;

	UPROPERTY()
	UGAS_GameplayAbilityBase* SelectedAttackCDO;
};
