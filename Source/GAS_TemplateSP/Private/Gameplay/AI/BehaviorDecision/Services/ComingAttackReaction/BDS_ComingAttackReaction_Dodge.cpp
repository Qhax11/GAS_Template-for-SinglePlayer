// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Dodge.h"

void UBDS_ComingAttackReaction_Dodge::InitializeAfterSelection()
{
    ApplyDirectionPoliciesToMovementAbility(DodgeMovementAbilityData);
}

bool UBDS_ComingAttackReaction_Dodge::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
    bool bIsUnDodgeableAttack = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
    return Super::IsEnable(ComingAttackPayload) && !bIsUnDodgeableAttack;
}

