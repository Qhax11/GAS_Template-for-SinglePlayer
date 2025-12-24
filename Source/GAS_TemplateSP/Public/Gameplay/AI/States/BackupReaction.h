// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "BackupReaction.generated.h"

/**
 * Temporary reaction state triggered when the player performs a finisher.
 *
 * While this state is active, nearby AI characters suspend their current
 * combat intentions and enter a short hesitation phase (?1–2 seconds).
 * During this phase they may idle, step back, or play a brief fear/avoidance
 * reaction instead of attacking.
 *
 * This is not a stun or hard crowd-control state; AI logic is only deferred,
 * not cancelled. Once the state ends, normal behavior evaluation resumes.
 *
 * Conceptually similar to the brief hesitation reactions seen in Sekiro
 * after execution/finisher events.
 */

UCLASS()
class GAS_TEMPLATESP_API UBackupReaction : public UStateBase
{
	GENERATED_BODY()
	
public:
	UBackupReaction();

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	UFUNCTION()
	void OnFinisherTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);
};
