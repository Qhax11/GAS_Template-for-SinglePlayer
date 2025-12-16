// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataDodge.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"


UComingAttackReactionDataDodge::UComingAttackReactionDataDodge()
{
    ComingAttackReactionName = FName("Dodge");
    ReactionType = EComingAttackReaction::Dodge;
    MinimumTimeBeforeHitToReact = 0.15f;    
    PreferredTriggerTimeBeforeHit = 0.25f;    
    BaseChance = 0.6f;

    // Dash distance
    DodgeMovementAbilityData.AbilityTriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Dodge;
	DodgeMovementAbilityData.EnableDirectionPolicy = true;
	DodgeMovementAbilityData.DirectionPolicyTag = GAS_Tags::TAG_AI_Direction_Policy_EscapeFromAttack;
    DodgeMovementAbilityData.AbilityEventMagnitude = 300.0f;
    DodgeMovementAbilityData.EnableDirectionPolicy = true;
    DodgeMovementAbilityData.ResolvedDirectionTag = GAS_Tags::TAG_Gameplay_Direction_Backward;
}

bool UComingAttackReactionDataDodge::IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug) const
{
	if (!Super::IsEnable(ComingAttackPayload, OutDebug))
	{
		return false;
	}

	const bool bIsUndodgeable = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
	if (bIsUndodgeable)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::UndodgeableAttack;
		}
		return false;
	}

	return true;
}

