// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h" // TODO: Try forward declaration.
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

public:
	// State Exit / Cleanup
	virtual void OnExit_Implementation() override;

protected:
	// Runtime State
	UGAS_GameplayAbilityBase* LastUsedAttack;
};
