// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"

void UMovementState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

bool UMovementState::EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	MovementStateEnterPayload = StaticCastSharedPtr<FMovementStatePayload>(EnterPayload);
	if (!MovementStateEnterPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UMovementState: MovementStateEnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	if (!MovementStateEnterPayload->SelectedAttackData->AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UMovementState: SelectedAttackData->AbilityClass is invalid in: %s"), *GetName());
		return false;
	}

	if (!IsValid(Enemy) || !IsValid(HeroTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UMovementState: Enemy or HeroTarget is invalid in: %s"), *GetName());
		return false;
	}

	return true;
}

void UMovementState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter();

	MovementStateEnterPayload = StaticCastSharedPtr<FMovementStatePayload>(EnterPayload);
	check(MovementStateEnterPayload.IsValid()); 
	check(MovementStateEnterPayload->SelectedAttackData);
	check(MovementStateEnterPayload->SelectedAttackData->AbilityClass);

	SelectedAttackCDO = MovementStateEnterPayload->SelectedAttackData->AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	StartMovementChain(MovementStateEnterPayload->SelectedMovementChainData);
}

void UMovementState::StartMovementChain(UMovementChainData* MovementChainData)
{
	if (!MovementChainData || !MovementManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UMovementState: MovementChainData or MovementManager is invalid in: %s"), *GetName());
		return;
	}

	if (!MovementManager->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManager->OnMovementChainEnded.AddDynamic(this, &UMovementState::OnMovementChainEnded);
	}

	MovementManager->ExecuteMovementChain(MovementChainData);
}

void UMovementState::OnTick_Implementation(float DeltaTime)
{
	TryEnterToAttackState();
}

void UMovementState::TryEnterToAttackState()
{
	if (bStepBackActive)
	{
		return;
	}

	if (!SelectedAttackCDO)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UMovementState: BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	EMovementRangeResult MovementRangeResult = EvaluateAttackRange();
	if (MovementRangeResult == EMovementRangeResult::TooClose) 
	{
		TryBackStep();
	}
	else if (MovementRangeResult == EMovementRangeResult::InRange) 
	{
		MovementManager->StopMovementAbilities();

		UAttackDataBase* SelectedAttackData = MovementStateEnterPayload->SelectedAttackData;
		TSharedPtr<FAttackStatePayload> AttackPayload = MakeShared<FAttackStatePayload>(SelectedAttackData);

		FStateTransitionRequest StateTransitionRequest = FStateTransitionRequest(GAS_Tags::TAG_AI_State_Attack, AttackPayload);
		ExitRequest("Target is in range", StateTransitionRequest);
	}
	else if (MovementRangeResult == EMovementRangeResult::TooFar) 
	{
		// Chain is continue for distance closing
	}
}

EMovementRangeResult UMovementState::EvaluateAttackRange() const
{
	if (!SelectedAttackCDO || !IsValid(Enemy) || !IsValid(HeroTarget))
	{
		return EMovementRangeResult::TooFar;
	}

	const float Distance = CombatDistance::GetDistance(Enemy, HeroTarget);
	const float MinRange = FMath::Max(0.f, SelectedAttackCDO->MinRange);
	const float MaxRange = SelectedAttackCDO->MaxRange;

	if (Distance < MinRange)
	{
		return EMovementRangeResult::TooClose;
	}

	if (Distance > MaxRange)
	{
		return EMovementRangeResult::TooFar;
	}

	return EMovementRangeResult::InRange;
}

void UMovementState::TryBackStep()
{
	const bool ExecutionSucces = MovementManager->ExecuteCorrectiveMovement(StepBackMovementData);
	if (ExecutionSucces)
	{
		MovementManager->OnMovementExecutionEnded.RemoveAll(this);
		MovementManager->OnMovementExecutionEnded.AddUObject(this, &UMovementState::OnBackStepEnded);
		bStepBackActive = true;
	}
}

void UMovementState::OnBackStepEnded(const FMovementExecutionEndedData& ReactionMovementEndedData)
{
	bStepBackActive = false;
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

