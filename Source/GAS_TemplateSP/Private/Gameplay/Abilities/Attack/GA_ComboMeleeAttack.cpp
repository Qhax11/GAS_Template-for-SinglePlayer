// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"

UGA_ComboMeleeAttack::UGA_ComboMeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_MeleeCombo);
}

void UGA_ComboMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UGA_ComboMeleeAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == GAS_Tags::TAG_Gameplay_AttackEvent_CanActivateNextAttack) 
	{
		OnCanExecuteNextAttack.Broadcast();
	}
	else 
	{
		Super::OnEventReceived(EventTag, EventData);
	}
}



