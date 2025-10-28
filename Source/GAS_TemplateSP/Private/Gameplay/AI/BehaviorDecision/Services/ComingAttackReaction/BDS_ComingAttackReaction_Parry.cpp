// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Parry.h"

bool UBDS_ComingAttackReaction_Parry::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
	bool bIsUnparryableAttack = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Unparryable);
	return Super::IsEnable(ComingAttackPayload) && !bIsUnparryableAttack;
}
