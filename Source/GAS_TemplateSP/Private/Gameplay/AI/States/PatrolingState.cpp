// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/PatrolingState.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_PatrolHandler.h"
#include "GameFramework/CharacterMovementComponent.h"

UPatrolingState::UPatrolingState()
{
	StateTag = GAS_Tags::TAG_AI_State_Patrolling;
}

void UPatrolingState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);

	check(Enemy);

	EnemyMovementComp = Enemy->GetCharacterMovement();
	check(EnemyMovementComp);

	PatrolHandler = Enemy->GetPatrolHandlerComponent();
	check(PatrolHandler);
}

void UPatrolingState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	if (!Enemy || !PatrolHandler )
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy or PatrolHandler is null!"));
		return;
	}

	PatrolHandler->OnPatrollingStopped.AddUObject(this,&UPatrolingState::OnPatrollingStopped);
	PatrolHandler->StartPatrolling(MovementData);
}

void UPatrolingState::OnPatrollingStopped()
{
	BroadcastTransition(FGameplayTag(), nullptr, "Patrolling is stopped.");
}

void UPatrolingState::StopEnemyMovement()
{
	if (!EnemyController || !EnemyMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UMovementState: EnemyController or EnemyMovementComp is null!"));
		return;
	}

	EnemyController->StopMovement();
	EnemyMovementComp->StopMovementImmediately();
}

void UPatrolingState::OnExit_Implementation()
{
	if (PatrolHandler) 
	{
		PatrolHandler->OnPatrollingStopped.RemoveAll(this);
		PatrolHandler->StopPatrolling();
	}

	StopEnemyMovement();

	Super::OnExit_Implementation();
}
