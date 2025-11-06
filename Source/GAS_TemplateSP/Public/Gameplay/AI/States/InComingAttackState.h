// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/Enemy/GA_EnemyTakeDamage.h"
#include "Gameplay/Abilities/InCombat/GA_ParryBase.h"
#include "Gameplay/Abilities/InCombat/GA_ParryKnockbackBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "InComingAttackState.generated.h"

struct FDamageData;

UCLASS()
class GAS_TEMPLATESP_API UInComingAttackState : public UStateBase
{
	GENERATED_BODY()

public:
	UInComingAttackState();

	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual bool EnterCondition_Implementation() override;

	virtual void OnEnter_Implementation() override;

	virtual void OnExit_Implementation() override;

protected:
	class US_DamageDelegates* DamageSubsystem;

	FTimerHandle DelayedReactionTimerHandle;

	virtual bool SelectAndMakeInComingAttackReaction();

	//********************* TAKE DAMAGE *********************/

	void BindTargetComingAttackEnd();

	void UnBindTargetComingAttackEnd();


	UFUNCTION()
	void OnDamageDealt(const FDamageData& DamageData);

	UFUNCTION()
	void OnTakeDamageAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_EnemyTakeDamage> EnemyTakeDamageAbilityClass;

	UGAS_GameplayAbilityBase* LastUsedTakeDamageAbility;

	UFUNCTION()
	void OnComingAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastComingAttackAbility;

	//********************* PARRY *********************/

	void MakeParryAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction);

	UFUNCTION()
	void OnParryAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_ParryBase> EnemyParryAbilityClass;

	UGAS_GameplayAbilityBase* LastUsedParryAbility;
	
	UFUNCTION()
	void OnParryKnocbackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_ParryKnockbackBase> EnemyParryKnocbackAbilityClass;

	UGAS_GameplayAbilityBase* LastUsedParryKnocbackAbility;
};
