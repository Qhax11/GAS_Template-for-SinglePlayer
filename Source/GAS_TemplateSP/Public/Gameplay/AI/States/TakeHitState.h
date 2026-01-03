// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "TakeHitState.generated.h"

class UGA_EnemyTakeDamage;
class UAC_EnemyMeleeComboManager;

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

	// Core execution
	virtual void TriggerTakeHitAbility(TSharedPtr<FTakeHitStatePayload> TakeHitPayload);

	// Callbacks
	void OnTakeHitAbilityEnded(const FCustomAbilityEndedData& AbilityEndedData);

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

	UPROPERTY()
	UAC_EnemyMeleeComboManager* ComboManager;
};
