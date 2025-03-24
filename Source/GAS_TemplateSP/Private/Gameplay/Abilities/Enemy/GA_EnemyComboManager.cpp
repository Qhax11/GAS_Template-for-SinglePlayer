// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/GA_EnemyComboManager.h"

UGA_EnemyComboManager::UGA_EnemyComboManager()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_EnemyComboManager::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateComboMeleeAttackAbility();
}

void UGA_EnemyComboManager::ActivateComboMeleeAttackAbility()
{
	if (TSubclassOf<UGA_ComboMeleeAttack> ComboAbilityClass = GetNextComboMeleeAttackAbility())
	{
		if (FGameplayAbilitySpec* SpecHandle = GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromClass(ComboAbilityClass))
		{
			if (UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Cast<UGA_ComboMeleeAttack>(SpecHandle->GetPrimaryInstance()))
			{
				if (GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(ComboAbilityClass))
				{
					
				}
			}
		}
	}
}

TSubclassOf<UGA_ComboMeleeAttack> UGA_EnemyComboManager::GetNextComboMeleeAttackAbility()
{
	if (ComboMeleeAttackAbilities.IsValidIndex(AbilityIndex))
	{
		return ComboMeleeAttackAbilities[AbilityIndex++];
	}

	else if (AbilityIndex > ComboMeleeAttackAbilities.Num() - 1)
	{
		if (ComboMeleeAttackAbilities.IsValidIndex(0))
		{
			AbilityIndex = 0;
			return ComboMeleeAttackAbilities[AbilityIndex++];
		}
	}

	return nullptr;
}

void UGA_EnemyComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
}