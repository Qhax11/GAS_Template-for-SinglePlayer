// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/StateTree/Tasks/STT_Test.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FPrintActorNameTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult&) const
{
	FStartStateInstanceData& Data = Context.GetInstanceData(*this);

	if (!Data.EnemyBase || !Data.StateClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateClass or Component is null."));
		return EStateTreeRunStatus::Failed;
	}

	if (UAC_StateManager* EnemyStateManagerComponent = Data.EnemyBase->GetEnemyStateManagerComponent()) 
	{
		Data.EnemyBase->GetEnemyStateManagerComponent()->StartStateByClass(Data.StateClass);
	}

	return EStateTreeRunStatus::Running;
}