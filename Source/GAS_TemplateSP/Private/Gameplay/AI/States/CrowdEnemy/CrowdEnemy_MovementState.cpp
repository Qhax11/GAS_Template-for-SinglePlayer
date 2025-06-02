// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/CrowdEnemy/CrowdEnemy_MovementState.h"
#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"

void UCrowdEnemy_MovementState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);

	MovementManagerComponent = Enemy->GetEnemyMovementManagerComponent();
	if (!MovementManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovementManagerComponent is null in: %s"), *GetName());
		return;
	}
}

void UCrowdEnemy_MovementState::OnEnter_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Movement State has been enter"));

	if (!BehaviorDecisionComponent)
	{
		return;
	}

	SelectedAttack = SelectNewAttackAbility();
	if (!SelectedAttack.AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid BestAttack selected."));
		ExitRequest(GAS_Tags::TAG_AI_State_Movement);
		return;
	}

	bStateFinished = false;

	SelectedAttackCDO = SelectedAttack.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	SelectMovement();
}

void UCrowdEnemy_MovementState::OnExit_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnExit"));

	if (MovementManagerComponent && MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UCrowdEnemy_MovementState::OnMovementChainEnded))
	{
		MovementManagerComponent->OnMovementChainEnded.RemoveDynamic(this, &UCrowdEnemy_MovementState::OnMovementChainEnded);
	}

	if (LastUsedStrafingAbility)
	{
		if (LastUsedStrafingAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UCrowdEnemy_MovementState::OnStrafingAbilityEnded))
		{
			LastUsedStrafingAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UCrowdEnemy_MovementState::OnStrafingAbilityEnded);
		}
		LastUsedStrafingAbility = nullptr;
	}
}

void UCrowdEnemy_MovementState::OnTick_Implementation(float DeltaTime)
{
	TryEnterToAttackState();
}

void UCrowdEnemy_MovementState::TryEnterToAttackState()
{
	if (!SelectedAttackCDO)
	{
		return;
	}

	if (IsAttackInRange(SelectedAttack.AbilityClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("attack ability is in range, exit from movement state"));
		MovementManagerComponent->StopMovementAbilities();
		ExitRequest(GAS_Tags::TAG_AI_State_Attack);
	}
}

void UCrowdEnemy_MovementState::SelectMovement()
{
	if (US_AICrowdEventManager* AICrowdEventManager = GetWorld()->GetGameInstance()->GetSubsystem<US_AICrowdEventManager>())
	{
		bool IsAttackIntender = AICrowdEventManager->RequestToBeAttackIntender(EnemyASC);
		if (IsAttackIntender) 
		{
			UE_LOG(LogTemp, Warning, TEXT("StartMovementChain"));
			StartMovementChain(SelectedAttack.AbilityClass);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MakeStrafingAbility"));
			MakeStrafingAbility();
		}
	}
}

void UCrowdEnemy_MovementState::StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackAbilityClass)
{
	MovementManagerComponent->StartMovementChain(SelectedAttackAbilityClass);

	if (!MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UCrowdEnemy_MovementState::OnMovementChainEnded))
	{
		MovementManagerComponent->OnMovementChainEnded.AddDynamic(this, &UCrowdEnemy_MovementState::OnMovementChainEnded);
	}
}

void UCrowdEnemy_MovementState::OnMovementChainEnded()
{
	ExitRequest();
}

void UCrowdEnemy_MovementState::MakeStrafingAbility()
{
	UGAS_GameplayAbilityBase* ActivatedStrafingAbility = EnemyASC->TryActivateAbilityByClassWithEventData(StrafingAbilityClass, StrafingAbilityEventData);
	if (ActivatedStrafingAbility)
	{
		if (!ActivatedStrafingAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UCrowdEnemy_MovementState::OnStrafingAbilityEnded))
		{
			ActivatedStrafingAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UCrowdEnemy_MovementState::OnStrafingAbilityEnded);
		}

		LastUsedStrafingAbility = ActivatedStrafingAbility;
	}
}

void UCrowdEnemy_MovementState::OnStrafingAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest();
}

