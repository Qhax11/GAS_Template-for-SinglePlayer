// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/PatrolingState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_PatrolHandler.h"

UPatrolingState::UPatrolingState()
{
	StateTag = GAS_Tags::TAG_AI_State_Patrolling;
}

void UPatrolingState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	if (!Enemy) 
	{
		return;
	}

	PatrolHandler = Enemy->GetPatrolHandlerComponent();
	if (!PatrolHandler)
	{
		return;
	}

	PatrolHandler->OnPatrollingStopped.AddUObject(this,&UPatrolingState::OnPatrollingStopped);
	PatrolHandler->StartPatrolling();
}

void UPatrolingState::OnPatrollingStopped()
{
	BroadcastTransition(FGameplayTag(), nullptr, "Patrolling is stopped.");
}

void UPatrolingState::OnExit_Implementation()
{
	if (PatrolHandler) 
	{
		PatrolHandler->OnPatrollingStopped.RemoveAll(this);
		PatrolHandler->StopPatrolling();
	}

	Super::OnExit_Implementation();
}
