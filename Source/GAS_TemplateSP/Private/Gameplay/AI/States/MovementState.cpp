// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"

void UMovementState::OnEnter(AGAS_EnemyBase* OwnerEnemy, AAIControllerBase* OwnerController)
{
	if (UAC_BehaviorDecision* OwnerBehaviorDecisionComp = OwnerController->GetBehaviorDecisionComponent())
	{
		FAttackData BestAttack = OwnerBehaviorDecisionComp->GetBestAttack();
		OwnerEnemy->GetEnemyMovementManagerComponent()->StartMovementChain(BestAttack.AbilityClass);
	}
}
