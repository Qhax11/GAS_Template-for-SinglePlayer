// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/BehaviorDecisionServiceBase.h"

void UBehaviorDecisionServiceBase::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
	Enemy = BehaviorServiceInitParams.Enemy;
	EnemyASC = BehaviorServiceInitParams.EnemyASC;
	Hero = BehaviorServiceInitParams.Hero;
	HeroMovementListenerComp = BehaviorServiceInitParams.HeroMovementListenerComp;
	BehaviorState = BehaviorServiceInitParams.BehaviorState;
	// The InAsset parameter will be cast to the appropriate asset type in subclasses.
}

float UBehaviorDecisionServiceBase::GetTargetDistance()
{
	if (!Enemy || !Hero)
	{
		return -1.f;
	}

	FVector MyLocation = Enemy->GetActorLocation();
	FVector TargetLocation = Hero->GetActorLocation();

	return FVector::Dist(MyLocation, TargetLocation);
}
