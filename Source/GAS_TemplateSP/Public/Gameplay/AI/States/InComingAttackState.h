// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "InComingAttackState.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UInComingAttackState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter() override;

	// Exit of take damage.
	UFUNCTION()
	void OnTakeDamageTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void ActivateParryAbility(FComingAttackReactionData BestComingAttackReaction);

	// Exit of Parry.
	UFUNCTION()
	void OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void ActivateDodgeAbility(FComingAttackReactionData BestComingAttackReaction);

	// Exit of Dodge.
	UFUNCTION()
	void OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);
};
