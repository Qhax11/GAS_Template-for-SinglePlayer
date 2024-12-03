// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ComboMeleeAttack.h"

UGA_ComboMeleeAttack::UGA_ComboMeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ComboMeleeAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);
}

UAnimMontage* UGA_ComboMeleeAttack::SelectSequence()
{
	return GetNextComboSequence();
}

UAnimMontage* UGA_ComboMeleeAttack::GetNextComboSequence()
{
	if (Montages.IsValidIndex(SequenceIndex))
	{
		return Montages[SequenceIndex++];
	}

	else if(Montages.IsValidIndex(0))
	{
		SequenceIndex = 0;
		return Montages[SequenceIndex++];
	}

	return nullptr;
}
