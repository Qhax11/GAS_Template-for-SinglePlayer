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
	if (!bIsEntered)
	{
		GenerateNewEntryID();
		bIsEntered = true;
	}
}

void UStateBase::OnExit()
{
	//bIsEntered = false;
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

FGuid UStateBase::GenerateNewEntryID()
{
	StateEntryID = FGuid::NewGuid();
	return StateEntryID;
}

bool UStateBase::IsThisEntryIDValid(FGuid EntryID) const
{
	return EntryID == StateEntryID;
}
