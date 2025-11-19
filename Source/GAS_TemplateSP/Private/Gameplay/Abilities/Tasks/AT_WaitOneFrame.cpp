// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tasks/AT_WaitOneFrame.h"

UAT_WaitOneFrame* UAT_WaitOneFrame::WaitOneFrame(UGameplayAbility* OwningAbility)
{
	UAT_WaitOneFrame* MyObj = NewAbilityTask<UAT_WaitOneFrame>(OwningAbility);
	return MyObj;
}

void UAT_WaitOneFrame::Activate()
{
	bTickingTask = true;
	bWaitedFrame = false;
}

void UAT_WaitOneFrame::TickTask(float DeltaTime)
{
	if (!bWaitedFrame)
	{
		// First tick → mark
		bWaitedFrame = true;
		return;
	}

	// Second tick → finished
	OnFinished.Broadcast();
	EndTask();
}