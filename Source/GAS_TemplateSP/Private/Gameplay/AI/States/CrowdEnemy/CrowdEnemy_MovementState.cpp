// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/CrowdEnemy/CrowdEnemy_MovementState.h"
#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"

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
			EnemyASC->TryActivateAbilityByClassWithEventData(ChaseAbilityClass, ChaseAbilityEventData);
			// Chase
		}
		else
		{
			// Strafing
		}
	}

}
