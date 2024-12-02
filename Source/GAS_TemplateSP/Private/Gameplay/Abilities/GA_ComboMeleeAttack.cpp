// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ComboMeleeAttack.h"

UGA_ComboMeleeAttack::UGA_ComboMeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ComboMeleeAttack::OnEventRecieved()
{
	Super::OnEventRecieved();
}

UAnimMontage* UGA_ComboMeleeAttack::SelectSequence()
{
	return GetNextComboSequence();
}

UAnimMontage* UGA_ComboMeleeAttack::GetNextComboSequence()
{
	if (AnimSequences.IsValidIndex(SequenceIndex)) 
	{
		return AnimSequences[SequenceIndex++];
	}

	else if(AnimSequences.IsValidIndex(0))
	{
		SequenceIndex = 0;
		return AnimSequences[SequenceIndex++];
	}

	return nullptr;
}
