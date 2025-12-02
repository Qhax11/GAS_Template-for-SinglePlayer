// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataDodge.h"

/*
void UComingAttackReactionDataDodge::InitializeAfterSelection()
{
    //ApplyDirectionPoliciesToMovementAbility(DodgeMovementAbilityData);
}
*/

UComingAttackReactionDataDodge::UComingAttackReactionDataDodge()
{
    ComingAttackReactionName = FName("Dodge");
    ReactionType = EComingAttackReaction::Dodge;
    MinimumTimeBeforeHitToReact = 0.15f;    
    PreferredTriggerTimeBeforeHit = 0.25f;    
    BaseChance = 0.6f;
}

bool UComingAttackReactionDataDodge::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
    bool bIsUnDodgeableAttack = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
    return Super::IsEnable(ComingAttackPayload) && !bIsUnDodgeableAttack;
}

