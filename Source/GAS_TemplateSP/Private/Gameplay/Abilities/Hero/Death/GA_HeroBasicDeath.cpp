// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Death/GA_HeroBasicDeath.h"

UGA_HeroBasicDeath::UGA_HeroBasicDeath()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_Death_Basic;
	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
}
