// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Router/GA_HeroRouter.h"

void UGA_HeroRouter::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InAir))
    {
        GetASC()->TryActivateAbilityByClassAndReturnInstance(KickAirAbilityClass);
    }
    else 
    {
        GetASC()->TryActivateAbilityByClassAndReturnInstance(JumpAbilityClass);
    }
   
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}