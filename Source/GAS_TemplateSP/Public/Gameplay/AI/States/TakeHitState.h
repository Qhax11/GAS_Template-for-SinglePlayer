// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "TakeHitState.generated.h"

class UGA_EnemyTakeDamage;

UCLASS()
class GAS_TEMPLATESP_API UTakeHitState : public UStateBase
{
	GENERATED_BODY()

public:
	UTakeHitState();

	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual bool EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload) override;
	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

protected:
	// Core execution
	virtual void ExecuteTakeHit(TSharedPtr<FTakeHitStatePayload> TakeHitPayload);

	// Callbacks
	void OnTakeHitAbilityEnded(const FCustomAbilityEndedData& AbilityEndedData);

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
	// State exit / cleanup
	virtual void OnExit_Implementation() override;

protected:
	// Config
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_EnemyTakeDamage> EnemyTakeDamageAbilityClass;

	// Runtime state
	UPROPERTY()
	UGAS_GameplayAbilityBase* LastUsedTakeDamageAbility = nullptr;

	bool bAbilityEnded = false;
	bool bPhaseTagCleared = false;
};
