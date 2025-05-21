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
	if (!BehaviorDecisionComponent || !MovementManagerComponent)
	{
		return;
	}

	BestAttack = BehaviorDecisionComponent->GetBestAttack();
	if (!BestAttack.AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid BestAttack selected."));
		ExitRequest();
		return;
	}

	AbilityCDO = BestAttack.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	StartMovementChain();
}

void UMovementState::OnTick(float DeltaTime)
{
	TryEnterToAttackState();
}

void UMovementState::OnExit()
{
	if (MovementManagerComponent && MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManagerComponent->OnMovementChainEnded.RemoveDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

void UMovementState::TryEnterToAttackState()
{
	if (AbilityCDO->MinRange > GetTargetDistance()) 
	{
		BestAttack = BehaviorDecisionComponent->GetBestAttack();
		AbilityCDO = BestAttack.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
		UE_LOG(LogTemp, Warning, TEXT("NEW ATTACK SELECTED"));
		StartMovementChain();
		return;
	}

	if (AbilityCDO->MaxRange > GetTargetDistance() && AbilityCDO->MinRange < GetTargetDistance())
	{
		ExitRequest();
	}
}

bool UMovementState::IsInRange()
{
	return false;
}

void UMovementState::StartMovementChain()
{
	MovementManagerComponent->StartMovementChain(BestAttack.AbilityClass);

	if (!MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded)) 
	{
		MovementManagerComponent->OnMovementChainEnded.AddDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

void UMovementState::OnMovementChainEnded()
{
	if (MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManagerComponent->OnMovementChainEnded.RemoveDynamic(this, &UMovementState::OnMovementChainEnded);
	}

	StartMovementChain();
}
