// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"

void UMovementState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UMovementState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter();

	MovementStateEnterPayload = StaticCastSharedPtr<FMovementStatePayload>(EnterPayload);
	if (!MovementStateEnterPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: MovementStateEnterPayload is invalid in: %s"), *GetName());
		return;
	}

	if (!MovementStateEnterPayload->SelectedAttackData->AbilityClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: SelectedAttackData->AbilityClass is invalid in: %s"), *GetName());
		return;
	}

	SelectedAttackCDO = MovementStateEnterPayload->SelectedAttackData->AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	ExecuteMovement(MovementStateEnterPayload);
}

void UMovementState::ExecuteMovement(TSharedPtr<FMovementStatePayload> MovementStatePayload)
{
	if (!MovementStatePayload.IsValid() || !MovementStatePayload->MovementChainAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: MovementStatePayload is invalid in: %s"), *GetName());
		return;
	}

	StartMovementChain(MovementStatePayload->MovementChainAsset);
}

void UMovementState::StartMovementChain(UMovementChainAsset* MovementChain)
{
	if (!MovementChain || !MovementManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: MovementStatePayload is invalid in: %s"), *GetName());
		return;
	}

	MovementManager->StartMovementChain(MovementChain);

	if (!MovementManager->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManager->OnMovementChainEnded.AddDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

void UMovementState::OnTick_Implementation(float DeltaTime)
{
	TryEnterToAttackState();
}

void UMovementState::TryEnterToAttackState()
{
	if (!SelectedAttackCDO)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	if (IsInRangeForAttack())
	{
		MovementManager->StopMovementAbilities();

		UAttackDataBase* SelectedAttackData = MovementStateEnterPayload->SelectedAttackData;
		TSharedPtr<FAttackStatePayload> AttackPayload = MakeShared<FAttackStatePayload>(SelectedAttackData);

		FStateTransitionRequest StateTransitionRequest = FStateTransitionRequest(GAS_Tags::TAG_AI_State_Attack, AttackPayload);
		ExitRequest("Target is in range", StateTransitionRequest);
	}

	/*
	if (SelectedAttackCDO->MinRange > EnemyController->GetTargetHeroDistance())
	{
		FAttackData NewAttack = BehaviorDecisionComponent->GetBestAttack();
		if (NewAttack.AbilityClass)
		{
			StartMovementChain(NewAttack.AbilityClass);
			return;
		}
	}
	*/
}

bool UMovementState::IsInRangeForAttack() const
{
	if (!SelectedAttackCDO || !EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: Ability Class is null in: %s"), *GetName());
		return false;
	}

	float Distance = EnemyController->GetTargetHeroDistance();
	return (SelectedAttackCDO->MaxRange > Distance && SelectedAttackCDO->MinRange < Distance);
}

void UMovementState::OnMovementChainEnded()
{
	ExitRequest("MovementChain is ended");
}

void UMovementState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (MovementManager && MovementManager->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManager->OnMovementChainEnded.RemoveDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

