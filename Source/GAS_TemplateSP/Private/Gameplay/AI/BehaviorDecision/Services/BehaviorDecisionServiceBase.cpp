// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"

void UBehaviorDecisionServiceBase::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
	Enemy = BehaviorServiceInitParams.Enemy;
	EnemyController = BehaviorServiceInitParams.EnemyController;
	EnemyASC = BehaviorServiceInitParams.EnemyASC;
	Hero = BehaviorServiceInitParams.Hero;
	HeroMovementListenerComp = BehaviorServiceInitParams.HeroMovementListenerComp;
	IntendManager = BehaviorServiceInitParams.IntendManager;
	// The InAsset parameter will be cast to the appropriate asset type in subclasses.
}

void UBehaviorDecisionServiceBase::InitializeAfterSelection()
{
    // The logic will be implement in subclasses.
}


