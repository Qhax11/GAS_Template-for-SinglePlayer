// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ComboManager.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"

void UGA_ComboManager::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Eðer combo seti boþsa, ability'i bitir
	if (ComboAbilityClasses.Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ASC = Cast<UGAS_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ActivateComboAttack();
}

void UGA_ComboManager::OnAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ComboIndex++;
	ActivateComboAttack();
}

void UGA_ComboManager::ActivateComboAttack()
{
	if (!ComboAbilityClasses.IsValidIndex(ComboIndex))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	TSubclassOf<UGA_ComboMeleeAttack> NextAbility = ComboAbilityClasses[ComboIndex];
	if (!NextAbility)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	LastActivatedCombo = ASC->TryActivateAbilityByClassAndReturnInstance(NextAbility);
	if (LastActivatedCombo)
	{
		if (!LastActivatedCombo->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UGA_ComboManager::OnAttackAbilityEnded))
		{
			LastActivatedCombo->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UGA_ComboManager::OnAttackAbilityEnded);
		}
	}
}

void UGA_ComboManager::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ComboIndex = 0;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
