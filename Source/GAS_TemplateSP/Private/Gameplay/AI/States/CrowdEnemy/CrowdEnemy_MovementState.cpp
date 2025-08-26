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

	AICrowdEventManager = GetWorld()->GetGameInstance()->GetSubsystem<US_AICrowdEventManager>();
	if (!AICrowdEventManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("AICrowdEventManager is null in: %s"), *GetName());
		return;
	}
}

void UCrowdEnemy_MovementState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();

	if (!BehaviorDecisionComponent)
	{
		return;
	}

	SelectedAttack = SelectNewAttackAbility();
	if (!SelectedAttack.AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid BestAttack selected."));
		ExitRequest("SelectedAttack is null", GAS_Tags::TAG_AI_State_Movement);
		return;
	}

	SelectedAttackCDO = SelectedAttack.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	SelectMovement();
}

void UCrowdEnemy_MovementState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (MovementManagerComponent && MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UCrowdEnemy_MovementState::OnMovementChainEnded))
	{
		MovementManagerComponent->OnMovementChainEnded.RemoveDynamic(this, &UCrowdEnemy_MovementState::OnMovementChainEnded);
	}

	if (LastUsedStrafingAbility && LastUsedStrafingAbility->IsValidLowLevel())
	{
		if (LastUsedStrafingAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UCrowdEnemy_MovementState::OnStrafingAbilityEnded))
		{
			LastUsedStrafingAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UCrowdEnemy_MovementState::OnStrafingAbilityEnded);
		}
		LastUsedStrafingAbility = nullptr;
	}

	GetWorld()->GetTimerManager().ClearTimer(WaitForNextStrafingOrbitTimerHandle);
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
		ExitRequest("Target is in range", GAS_Tags::TAG_AI_State_Attack);
	}
}

void UCrowdEnemy_MovementState::SelectMovement()
{
	if (!AICrowdEventManager) 
	{
		UE_LOG(LogTemp, Warning, TEXT("AICrowdEventManager is null in: %s"), *GetName());
		return;
	}

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
	ExitRequest("OnMovementChainEnded");
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
	// Delay before make strafing again
	float WaitTime = FMath::RandRange(MinStrafingWaitTime, MaxStrafingWaitTime);
	GetWorld()->GetTimerManager().SetTimer(WaitForNextStrafingOrbitTimerHandle, this, &UCrowdEnemy_MovementState::OnWaitTimeFinished, WaitTime, false);
}

void UCrowdEnemy_MovementState::OnWaitTimeFinished()
{
	ExitRequest("OnWaitTimeFinished");
}


