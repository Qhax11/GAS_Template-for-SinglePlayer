// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_MeleeFinisher.h"

UGA_MeleeFinisher::UGA_MeleeFinisher()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Finisher);
	SetAssetTags(AssetTags);
	//AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Finisher);
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Finisher);
}
