// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"

void UAC_EnemyMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	Super::OnComboMeleeAttackAbilityEnd(EndedData);

	// This means we've reached the end of the combo
	if (AbilityIndex == ComboMeleeAttackAbilities.Num() - 1)
	{
		OnComboEnded.Broadcast();
	}
}

void UAC_EnemyMeleeComboManager::ActivateComboMeleeAttackAbilityWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass)
{
	AbilityIndex = ComboMeleeAttackAbilities.Find(ComboMeleeAttackAbilityClass);
	ActivateComboMeleeAttackAbility();
}
