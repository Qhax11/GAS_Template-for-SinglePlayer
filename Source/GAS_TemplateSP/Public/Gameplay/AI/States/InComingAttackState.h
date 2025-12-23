// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
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

	virtual bool EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void OnExit_Implementation() override;

protected:
	TSharedPtr<FIncomingAttackStatePayload> InComingAttackStatePayload;

	class US_DamageDelegates* DamageSubsystem;

	FTimerHandle DelayedReactionTimerHandle;

	virtual bool SelectAndExecuteReaction(UComingAttackReactionData* SelectedReactionData);

	//********************* TAKE DAMAGE *********************/

	void BindTargetComingAttackEnd();

	void UnBindTargetComingAttackEnd();

	UFUNCTION()
	void OnDamageDealt(const FDamageData& DamageData);

	UFUNCTION()
	void OnComingAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	UPROPERTY()
	UGAS_GameplayAbilityBase* LastComingAttack;

	//********************* PARRY *********************/

	void ExecuteParry(const UComingAttackReactionData* BestComingAttackReaction);

	void ExecuteParryKnocback(const FDamageData& DamageData);

	void FaceTargetBeforeParry();

	UFUNCTION()
	void OnParryAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_ParryBase> EnemyParryAbilityClass;

	UPROPERTY()
	UGAS_GameplayAbilityBase* LastUsedParry;
	
	UFUNCTION()
	void OnParryKnocbackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_ParryKnockbackBase> EnemyParryKnocbackAbilityClass;

	UPROPERTY()
	UGAS_GameplayAbilityBase* LastUsedParryKnocback;

	void CleanupDelegates();

protected:
	FDelegateHandle ComingAttackEndHandle;
	FDelegateHandle TakeDamageEndHandle;
	FDelegateHandle ParryEndHandle;
	FDelegateHandle ParryKnockbackEndHandle;
	FDelegateHandle DamageSubsystemHandle;
};
