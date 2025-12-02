// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataDodge.h"

/*
void UComingAttackReactionDataDodge::InitializeAfterSelection()
{
    //ApplyDirectionPoliciesToMovementAbility(DodgeMovementAbilityData);
}
*/

bool UComingAttackReactionDataDodge::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
    bool bIsUnDodgeableAttack = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
    return Super::IsEnable(ComingAttackPayload) && !bIsUnDodgeableAttack;
}

