// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"

void UInComingAttackState::OnEnter()
{
	FComingAttackReactionData BestComingAttackReaction = BehaviorDecisionComponent->GetBestComingAttackDecision(StateManager->ComingAttackPayload);
	
	ActivateParryAbility(BestComingAttackReaction);
}

void UInComingAttackState::ActivateParryAbility(FComingAttackReactionData BestComingAttackReaction)
{
	UGAS_GameplayAbilityBase* ActivatedAbility =
		EnemyASC->TryActivateAbilityByClassAndReturnInstance(BestComingAttackReaction.RecationAbilityClass);

	//ActivatedAbility->OnGameplayAbilityEndedWithData.AddUObject(this, &BestComingAttackReaction.RecationAbilityClass);
}


