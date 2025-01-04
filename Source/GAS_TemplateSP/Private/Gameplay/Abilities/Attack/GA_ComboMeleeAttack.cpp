// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"

void UGA_ComboMeleeAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}



