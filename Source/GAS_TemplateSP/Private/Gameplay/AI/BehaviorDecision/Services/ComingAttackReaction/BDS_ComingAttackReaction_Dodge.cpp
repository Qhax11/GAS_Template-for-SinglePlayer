// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Dodge.h"

void UBDS_ComingAttackReaction_Dodge::InitializeAfterSelection()
{
    ApplyDirectionPoliciesToMovementAbility(DodgeMovementAbilityData);
}

