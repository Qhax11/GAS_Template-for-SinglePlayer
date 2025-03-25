// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"

void UAC_EnemyMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	Super::OnComboMeleeAttackAbilityEnd(EndedData);

	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		return;
	}

	// This means we've reached the end of the combo
	if (AbilityIndex == ComboMeleeAttackAbilities.Num())
	{
		OnComboEnded.Broadcast();
	}
	else
	{
		ActivateComboMeleeAttackAbility();
	}
}

void UAC_EnemyMeleeComboManager::ActivateComboMeleeAttackAbilityWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass)
{
	AbilityIndex = ComboMeleeAttackAbilities.Find(ComboMeleeAttackAbilityClass);
	ActivateComboMeleeAttackAbility();
}
