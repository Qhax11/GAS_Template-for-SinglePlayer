// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataParry.h"

UComingAttackReactionDataParry::UComingAttackReactionDataParry()
{
	ComingAttackReactionName = FName("Parry");
	ReactionType = EComingAttackReaction::Parry;
	MinimumTimeBeforeHitToReact = 0.2f;	
	PreferredTriggerTimeBeforeHit = 0.3f;	
	BaseChance = 0.7f;
}

bool UComingAttackReactionDataParry::IsEnable(UComingAttackReactionData* ComingReactionData, FComingAttackPayload ComingAttackPayload) const
{
	bool bIsUnparryableAttack = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Unparryable);
	return Super::IsEnable(ComingReactionData, ComingAttackPayload) && !bIsUnparryableAttack;
}

