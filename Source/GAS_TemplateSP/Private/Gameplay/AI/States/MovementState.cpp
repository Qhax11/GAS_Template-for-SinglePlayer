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

	FAttackData SelectedBestAttack = GetSelectedAttackAbility();
	if (!SelectedBestAttack.AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid BestAttack selected."));
		ExitRequest(GAS_Tags::TAG_AI_StateTreeEvent_Transaction_MovementState_Exit);
		return;
	}

	bStateFinished = false;

	SelectedAttackCDO = SelectedBestAttack.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	StartMovementChain(SelectedBestAttack.AbilityClass);
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
	if (!SelectedAttackCDO)
	{
		return;
	}

	if (SelectedAttackCDO->MinRange > EnemyController->GetTargetHeroDistance())
	{
		BestAttack = BehaviorDecisionComponent->GetBestAttack();
		SelectedAttackCDO = BestAttack.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
		UE_LOG(LogTemp, Warning, TEXT("NEW ATTACK SELECTED"));
		//StartMovementChain(SelectedAttackCDO->GetClass());
		return;
	}

	if (SelectedAttackCDO->MaxRange > EnemyController->GetTargetHeroDistance() && SelectedAttackCDO->MinRange < EnemyController->GetTargetHeroDistance())
	{
		UE_LOG(LogTemp, Warning, TEXT("attack ability is in range, exit from movement state"));
		MovementManagerComponent->StopMovementAbilities();
		ExitRequest(GAS_Tags::TAG_AI_StateTreeEvent_Transaction_AttackState_Enter);
	}
}

bool UMovementState::IsInRange()
{
	return false;
}

void UMovementState::StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackAbilityClass)
{
	MovementManagerComponent->StartMovementChain(SelectedAttackAbilityClass);

	if (!MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded)) 
	{
		MovementManagerComponent->OnMovementChainEnded.AddDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

void UMovementState::OnMovementChainEnded()
{
	ExitRequest(GAS_Tags::TAG_AI_StateTreeEvent_Transaction_MovementState_Enter);
}
