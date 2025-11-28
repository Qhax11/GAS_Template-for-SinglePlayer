// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroFinisher.h"

UGA_HeroFinisher::UGA_HeroFinisher()
{
	ActivationBlockedTags.RemoveTag(GAS_Tags::TAG_Gameplay_State_Phase_Active);
}
