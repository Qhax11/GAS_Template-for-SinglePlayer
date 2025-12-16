// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataTakeHit.h"
#include "AbilitySystemGlobals.h"

UComingAttackReactionDataTakeHit::UComingAttackReactionDataTakeHit()
{
	ComingAttackReactionName = FName("TakeHit");
	ReactionType = EComingAttackReaction::TakeDamage;
	MinimumTimeBeforeHitToReact = 0.0f;	
	PreferredTriggerTimeBeforeHit = 0.1f;	

	BaseChance = 1.0f;
	MinChance = 1.0f;
	MaxChance = 1.0f;
}

bool UComingAttackReactionDataTakeHit::IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug) const
{
	if (!Super::IsEnable(ComingAttackPayload, OutDebug))
	{
		return false;
	}

	const bool bDefenderUnstoppable = ComingAttackPayload.DefenderASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack);
	if (!bDefenderUnstoppable)
	{
		return true;
	}

	// Can attack interrupt unstoppable state?
	const bool bAttackCanInterruptUnstoppable = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_CanInterruptUnstoppable);
	if (!bAttackCanInterruptUnstoppable)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::Unstoppable;
		}
		return false;
	}

	return true;
}


