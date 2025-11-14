// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Router/GA_HeroRouter.h"

void UGA_HeroRouter::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilitiesToTry)
    {
        if (!AbilityClass) 
        {
            continue;
        }

        bool bActivated = GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(AbilityClass);

        if (bActivated)
        {
            EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
            return;
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}