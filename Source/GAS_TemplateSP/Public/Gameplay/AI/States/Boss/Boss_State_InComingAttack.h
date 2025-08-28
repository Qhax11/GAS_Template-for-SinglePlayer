// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/InComingAttackState.h"
#include "Boss_State_InComingAttack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UBoss_State_InComingAttack : public UInComingAttackState
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter_Implementation() override;

	virtual bool SelectAndMakeInComingAttackReaction() override;

	void ActivateDodgeAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction);

	UFUNCTION()
	void OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastUsedDodgeAbility;

	virtual void OnExit_Implementation() override;
};
