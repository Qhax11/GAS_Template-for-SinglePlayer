// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeFinisher.h"

UGA_MeleeFinisher::UGA_MeleeFinisher()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher);
}

FGameplayTag UGA_MeleeFinisher::GetFinisherTypeTagFromAbilityTags() const
{
	for (const FGameplayTag& Tag : AbilityTags)
	{
		if (Tag.MatchesTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

