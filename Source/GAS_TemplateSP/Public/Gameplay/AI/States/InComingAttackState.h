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

	virtual void OnEnter_Implementation() override;

	virtual void OnExit_Implementation() override;

protected:
	FTimerHandle DelayedReactionTimerHandle;

	void SelectAndMakeInComingAttackReaction();

	void TriggerIncomingReaction(FComingAttackReactionData Reaction);

	//********************* TAKE DAMAGE *********************/

	void MakeTakeDamage(FComingAttackReactionData BestComingAttackReaction);

	UFUNCTION()
	void OnTargetAbilityActivated(UGameplayAbility* Ability);

	// Exit of Take Damage.
	UFUNCTION()
	void OnTakeDamageAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastUsedTakeDamageAbility;

	//********************* PARRY *********************/

	void MakeParryAbility(FComingAttackReactionData BestComingAttackReaction);

	UFUNCTION()
	void OnParryTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnParryKnocbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	bool bParryKnockbackHappened = false;

	//********************* DODGE *********************/

	void ActivateDodgeAbility(FComingAttackReactionData BestComingAttackReaction);

	// Exit of Dodge.
	UFUNCTION()
	void OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastUsedDodgeAbility;

};
