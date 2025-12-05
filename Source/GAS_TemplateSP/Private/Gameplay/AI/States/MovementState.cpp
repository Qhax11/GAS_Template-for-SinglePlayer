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
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MovementManagerComponent is null in: %s"), *GetName());
		return;
	}
}

void UMovementState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter();

	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	FAttackData NewAttack = BehaviorDecisionComponent->GetBestAttack();
	if (!NewAttack.AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: No valid BestAttack selected."));
		ExitRequest("SelectedAttack ability class is null");
		return;
	}

	StartMovementChain(NewAttack.AbilityClass);
}

void UMovementState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (MovementManagerComponent && MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManagerComponent->OnMovementChainEnded.RemoveDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

void UMovementState::OnTick_Implementation(float DeltaTime)
{
	TryEnterToAttackState();
}

void UMovementState::TryEnterToAttackState()
{
	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	if (GetSelectedAttackAbilityCDO()->MinRange > EnemyController->GetTargetHeroDistance())
	{
		FAttackData NewAttack = BehaviorDecisionComponent->GetBestAttack();
		if (NewAttack.AbilityClass) 
		{
			StartMovementChain(NewAttack.AbilityClass);
			return;
		}
	}

	if (IsAttackInRange(BehaviorDecisionComponent->LastSelectedAttackData.AbilityClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: attack ability is in range, exit from movement state"));
		MovementManagerComponent->StopMovementAbilities();
		ExitRequest("Target is in range", GAS_Tags::TAG_AI_State_Attack);
	}
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
	ExitRequest("MovementChain is ended", GAS_Tags::TAG_AI_State_Movement);
}
