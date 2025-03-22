// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/StateTree/Tasks/STT_Test.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FPrintActorNameTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult&) const
{
	FTestPrintActorNameTaskInstanceData& Data = Context.GetInstanceData(*this);

	if (Data.TargetActor)
	{
		UE_LOG(LogTemp, Log, TEXT("Target: %s"), *Data.TargetActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is NULL"));
	}

	return EStateTreeRunStatus::Succeeded;
}