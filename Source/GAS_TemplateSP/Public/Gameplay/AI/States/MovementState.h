// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "MovementState.generated.h"

class UMovementChainDataa;
class UMovementSingleData;

/**
 * UMovementState
 *
 * AI state responsible for **positioning the enemy for a selected attack**.
 *
 * Conceptual role:
 * This state handles all movement logic required to bring the enemy into a
 * valid execution range for a previously selected attack.
 *
 * Responsibilities:
 * - Receive the selected attack + movement chain via FMovementStatePayload
 * - Execute the chosen movement chain through the EnemyMovementManager
 * - Continuously evaluate distance to the target during movement
 * - Transition to AttackState once the attack’s valid range is reached
 *
 * What this state does NOT do:
 * - It does NOT select which attack to use (handled by BehaviorDecision)
 * - It does NOT execute the attack ability itself (handled by AttackState)
 * - It does NOT score or filter movement chains (handled by MovementChainData)
 *
 * Design notes:
 * - Distance checks here are **state-level execution checks**, not decision filters.
 * - The attack may have been selected regardless of distance; this state’s job
 *   is to make that attack executable.
 * - Movement chains are treated as transient helpers, not long-lived behaviors.
 * - If movement ends early or conditions change, the state exits and the
 *   StateManager re-evaluates the next state.
 *
 * Typical flow:
 *   BehaviorDecision ? Attack selected
 *   StateManager     ? Enter MovementState
 *   MovementState    ? Execute movement chain
 *   Tick             ? Check attack range
 *   Range satisfied  ? Transition to AttackState
 *
 * This state is intentionally generic and reusable:
 * - Works for melee, ranged, boss, or special attacks
 * - Supports different movement chain strategies (dash, strafe, chase, etc.)
 * - Keeps positioning logic isolated from attack execution
 */
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
