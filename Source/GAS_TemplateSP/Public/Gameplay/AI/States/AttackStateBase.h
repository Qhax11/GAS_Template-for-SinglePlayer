// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AttackStateBase.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UAttackStateBase : public UStateBase
{
	GENERATED_BODY()
	
public:
	UAttackStateBase();

	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

protected:
	// Core Execution
	virtual bool SelectAndMakeAttack(TSharedPtr<FAttackStatePayload> AttackStatePayload);

	void MakeAttack(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAttackClass);

	// Callbacks
	void OnAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	UFUNCTION()
	void OnActivePhasePostHitTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	// Exit condition for phase-based states:
    // State exits ONLY when:
    // 1) The triggered ability has ended (logic lifecycle)
    // 2) All relevant animation phase tags have been cleared (visual / gameplay lifecycle)
    //
    // This prevents early state transitions during montage blend-out
    // while animation phases are still active.
	void TryExitState();

public:
	// State Exit / Cleanup
	virtual void OnExit_Implementation() override;

protected:
	// Runtime State
	UGAS_GameplayAbilityBase* LastUsedAttack;

	bool bAbilityEnded = false;
	bool bPhaseTagCleared = false;

};
