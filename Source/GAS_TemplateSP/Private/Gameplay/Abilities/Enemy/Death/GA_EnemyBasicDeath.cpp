// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Death/GA_EnemyBasicDeath.h"

UGA_EnemyBasicDeath::UGA_EnemyBasicDeath()
{
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_DeadWithFinisher);
}
