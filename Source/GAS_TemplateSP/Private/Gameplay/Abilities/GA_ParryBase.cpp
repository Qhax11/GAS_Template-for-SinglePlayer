// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ParryBase.h"

UGA_ParryBase::UGA_ParryBase()
{
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Parry);
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Parry);
}

void UGA_ParryBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_ParryBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


