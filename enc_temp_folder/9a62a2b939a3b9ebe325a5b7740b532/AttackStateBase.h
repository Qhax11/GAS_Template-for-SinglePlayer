// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h" // TODO: Try forward declaration.
#include "AttackStateBase.generated.h"

/**
 * UAttackStateBase
 *
 * Base AI state responsible for **executing a selected attack ability**.
 *
 * Conceptual role:
 * This state represents the moment where the AI commits to an attack that has
 * already been selected by the behavior-decision layer.
 *
 * Responsibilities:
 * - Validate the incoming attack payload (FAttackStatePayload)
 * - Ensure required runtime context exists (Enemy, HeroTarget, AbilityClass)
 * - Verify that the target is still within executable attack range
 * - Activate the selected Gameplay Ability via the Ability System
 * - Listen for the ability end event and exit the state accordingly
 *
 * What this state does NOT do:
 * - It does NOT select which attack to use (handled by BehaviorDecision)
 * - It does NOT score or filter attacks (handled by AttackData / services)
 * - It does NOT manage movement or positioning (handled by MovementState)
 *
 * Design notes:
 * - Range checking here is an execution-time validation, not a selection filter.
 * - This allows attacks to be chosen independently of distance, while still
 *   preventing invalid execution.
 * - The state assumes EnterCondition has already been approved by the StateManager
 *   before OnEnter is called.
 *
 * Typical flow:
 *   BehaviorDecision ? Attack selected
 *   StateManager     ? EnterCondition()
 *   AttackState      ? OnEnter() ? ExecuteAttack()
 *   Ability ends     ? OnAttackAbilityEnded() ? ExitRequest()
 *
 * This class is intended to be subclassed for specialized behaviors
 * (e.g. boss attacks, combo logic, cinematic attacks),
 * while keeping the core execution contract consistent.
 */

class UAttackDataBase;
class UComboChainAttackData;
class USpecialAttackData;
class UAC_EnemyMeleeComboManager;

UCLASS()
class GAS_TEMPLATESP_API UAttackStateBase : public UStateBase
{
	GENERATED_BODY()
	
public:
	UAttackStateBase();

	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual bool EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload = nullptr) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

protected:
	void ExecuteAttack(UAttackDataBase* SelectedAttackData);

	void ExecuteSpecialAttack(USpecialAttackData* SpecialAttackData);

	void ExecuteComboAttack(UComboChainAttackData* ComboData);

	// Callbacks
	void OnAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

public:
	// State Exit / Cleanup
	virtual void OnExit_Implementation() override;

protected:
	// Runtime State
	UGAS_GameplayAbilityBase* LastUsedAttack;

private:
	UPROPERTY()
	UAC_EnemyMeleeComboManager* ComboManager;
};
