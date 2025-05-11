// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"

void UMovementState::OnEnter(AGAS_EnemyBase* OwnerEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Movement State has been enter"));
	//OwnerEnemy->GetEnemyMovementManagerComponent()->StartMovementChain()
}
