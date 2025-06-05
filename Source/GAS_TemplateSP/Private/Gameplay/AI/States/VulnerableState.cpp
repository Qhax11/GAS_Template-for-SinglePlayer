// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/VulnerableState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"


void UVulnerableState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();

	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

	EnemyController->GetEnemyStateManagerComponent()->StopLogic();

	EnemyASC()
}
