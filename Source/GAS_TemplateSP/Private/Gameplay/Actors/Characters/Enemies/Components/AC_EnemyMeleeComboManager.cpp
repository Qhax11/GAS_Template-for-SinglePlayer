// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"

void UAC_EnemyMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	Super::OnComboMeleeAttackAbilityEnd(EndedData);

	// If it is another ability
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		return;
	}

	// If ComboMelee ability is cancelled
	if (EndedData.bWasCancelled)
	{
		AbilityIndex = 0;
	}
	else
	{

	}

	bCanActivateAbility = true;

	OnComboMeleeEnded.Broadcast(EndedData.bWasCancelled);
}
