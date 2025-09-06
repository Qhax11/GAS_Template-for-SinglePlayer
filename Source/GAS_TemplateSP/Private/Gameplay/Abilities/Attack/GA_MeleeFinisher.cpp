// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_MeleeFinisher.h"

UGA_MeleeFinisher::UGA_MeleeFinisher()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher);
}

void UGA_MeleeFinisher::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

