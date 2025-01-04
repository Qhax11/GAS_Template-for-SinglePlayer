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

	if (TSubclassOf<UGameplayAbility> ComboAbilityClass = GetNextComboMeleeAttackAbility())
	{
		if (GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(ComboAbilityClass))
		{
			FGameplayAbilitySpec* CurrentAbilitySpecHandle = GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromClass(ComboAbilityClass);
			if (UGA_ComboMeleeAttack* ComboAbility = Cast<UGA_ComboMeleeAttack>(CurrentAbilitySpecHandle->Ability))
			{
				ComboAbility->OnCanExecuteNextAttack.AddDynamic(this, &UGA_ComboMeleeAttackManager::OnCanExecuteNextAttack);
				GetAbilitySystemComponentFromActorInfo()->OnAbilityEnded.AddUObject(this, &UGA_ComboMeleeAttackManager::OnComboMeleeAttackAbilityEnd);
			}
		}
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
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		return;
	}

	if (!EndedData.bWasCancelled)
	{
		AbilityIndex = 0;
	}
}

void UGA_ComboMeleeAttackManager::OnCanExecuteNextAttack()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
