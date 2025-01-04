// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttackManager.h"

UGA_ComboMeleeAttackManager::UGA_ComboMeleeAttackManager()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ComboMeleeAttackManager::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComponent)
	{
		TSubclassOf<UGameplayAbility> ComboAbilityClass = GetNextComboMeleeAttackAbility();
		AbilitySystemComponent->TryActivateAbilityByClass(ComboAbilityClass);
		AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UGA_ComboMeleeAttackManager::OnComboMeleeAttackAbilityEnd);
	}

}

TSubclassOf<UGA_ComboMeleeAttack> UGA_ComboMeleeAttackManager::GetNextComboMeleeAttackAbility()
{
	if (ComboMeleeAttackAbilities.IsValidIndex(AbilityIndex))
	{
		return ComboMeleeAttackAbilities[AbilityIndex++];
	}

	else if (ComboMeleeAttackAbilities.IsValidIndex(0))
	{
		AbilityIndex = 0;
		return ComboMeleeAttackAbilities[AbilityIndex++];
	}

	return nullptr;
}

void UGA_ComboMeleeAttackManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
