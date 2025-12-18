// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/InComingAttackState.h"
#include "Boss_State_InComingAttack.generated.h"

struct FReactionMovementEndedData;

UCLASS()
class GAS_TEMPLATESP_API UBoss_State_InComingAttack : public UInComingAttackState
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual bool SelectAndExecuteReaction(UComingAttackReactionData* SelectedReactionData) override;

	void ActivateDodgeAbility(UComingAttackReactionData* SelectedBestReaction);

	void OnDodgeExecutionEnded(const FMovementExecutionEndedData& ReactionMovementEndedData);

	UGAS_GameplayAbilityBase* LastUsedDodgeAbility;

	virtual void OnExit_Implementation() override;
};
