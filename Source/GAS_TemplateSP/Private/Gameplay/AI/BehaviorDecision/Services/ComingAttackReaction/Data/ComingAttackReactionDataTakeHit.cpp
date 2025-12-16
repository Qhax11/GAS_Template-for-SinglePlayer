// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataTakeHit.h"

UComingAttackReactionDataTakeHit::UComingAttackReactionDataTakeHit()
{
	ComingAttackReactionName = FName("TakeHit");
	ReactionType = EComingAttackReaction::TakeDamage;
	MinimumTimeBeforeHitToReact = 0.0f;	
	PreferredTriggerTimeBeforeHit = 0.1f;	
	BaseChance = 1.0f;
}

bool UComingAttackReactionDataTakeHit::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
	return true;
}


