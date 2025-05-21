// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"

void UStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Enemy = StateInitParams.Enemy;
	EnemyController = StateInitParams.EnemyController;
	EnemyASC = StateInitParams.EnemyASC;
	BehaviorDecisionComponent = StateInitParams.BehaviorDecisionComponent;
	StateManager = StateInitParams.StateManager;
}

void UStateBase::OnEnter()
{
	
}

void UStateBase::OnExit()
{
}

void UStateBase::ExitRequest()
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	StateManager->RequestStateTreeExit(this);
}

float UStateBase::GetTargetDistance() const
{
	if (!Enemy || !EnemyController || !EnemyController->GetTarget())
	{
		return -1.0f;
	}

	FVector MyLocation = Enemy->GetActorLocation();
	FVector TargetLocation = EnemyController->GetTarget()->GetActorLocation();

	return FVector::Dist(MyLocation, TargetLocation);
}

