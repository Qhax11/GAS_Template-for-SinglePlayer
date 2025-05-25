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

void UStateBase::ExitRequest(const FGameplayTag& TransactionTag)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	if (!bStateFinished)
	{
		bStateFinished = true;
		StateManager->RequestStateTreeExit(this, TransactionTag);
	}
}

FAttackData UStateBase::GetSelectedAttackAbility() const
{
	return StateManager->LastSelectedAttackData;
}

FAttackData UStateBase::SelectNewAttackAbility() const
{
	return StateManager->SelectNewBestAttack();
}

