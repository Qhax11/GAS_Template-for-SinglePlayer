// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/StateBase.h"

void UStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Enemy = StateInitParams.Enemy;
	EnemyController = StateInitParams.EnemyController;
	EnemyASC = StateInitParams.EnemyASC;
	BehaviorDecisionComponent = StateInitParams.BehaviorDecisionComponent;
}
