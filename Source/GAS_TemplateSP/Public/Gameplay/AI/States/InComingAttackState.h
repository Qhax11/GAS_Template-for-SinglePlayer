// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/Enemy/GA_EnemyTakeDamage.h"
#include "InComingAttackState.generated.h"

struct FDamageData;

UCLASS()
class GAS_TEMPLATESP_API UInComingAttackState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter_Implementation() override;

	virtual void OnExit_Implementation() override;

protected:
	class US_DamageDelegates* DamageSubsystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_EnemyTakeDamage> EnemyTakeDamage;

	FTimerHandle DelayedReactionTimerHandle;

	void SelectAndMakeInComingAttackReaction();

	//********************* TAKE DAMAGE *********************/

	void MakeTakeDamage(const UBDS_ComingAttackReactionBase* BestComingAttackReaction);

	UFUNCTION()
	void OnDamageDealt(const FDamageData& DamageData);

	UFUNCTION()
	void OnTakeDamageAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	void OnTakeDamageFailsafeTimeout();
	FTimerHandle TakeDamageFailsafeTimer;
	UGAS_GameplayAbilityBase* LastUsedTakeDamageAbility;

	//********************* PARRY *********************/

	void MakeParryAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction);

	UFUNCTION()
	void OnParryTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnParryKnocbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	const class UBDS_ComingAttackReaction_Parry* BDS_Parry;
	bool bParryKnockbackHappened = false;

	//********************* DODGE *********************/

	void ActivateDodgeAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction);

	// Exit of Dodge.
	UFUNCTION()
	void OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastUsedDodgeAbility;



};
