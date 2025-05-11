// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackState.h"

void UAttackState::OnEnter(AGAS_EnemyBase* OwnerEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack State has been enter"));
	//OwnerEnemy->GetEnemyMovementManagerComponent()->StartMovementChain()
}