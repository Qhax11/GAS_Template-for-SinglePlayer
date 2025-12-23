// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_Boss_SpecialAttackBase.h"

UGA_Boss_SpecialAttackBase::UGA_Boss_SpecialAttackBase()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack);
}
