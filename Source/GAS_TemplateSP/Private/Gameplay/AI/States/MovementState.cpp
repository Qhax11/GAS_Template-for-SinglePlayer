// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"

void UMovementState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);

	MovementManagerComponent = Enemy->GetEnemyMovementManagerComponent();
	if (!MovementManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovementManagerComponent is null in: %s"), *GetName());
		return;
	}

}

void UMovementState::OnEnter()
{
	StartMovementChain();
}

void UMovementState::OnExit()
{
}

void UMovementState::StartMovementChain()
{
	if (!BehaviorDecisionComponent || !MovementManagerComponent)
	{
		return;
	}

	FAttackData BestAttack = BehaviorDecisionComponent->GetBestAttack();
	MovementManagerComponent->StartMovementChain(BestAttack.AbilityClass);
	MovementManagerComponent->OnMovementChainEnded.AddDynamic(this, &UMovementState::OnMovementChainEnded);
}

void UMovementState::OnMovementChainEnded()
{
	MovementManagerComponent->OnMovementChainEnded.RemoveDynamic(this, &UMovementState::OnMovementChainEnded);
	StartMovementChain();
}
