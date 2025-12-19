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

	const UGAS_GameplayAbilityBase* AttackCDO = MovementStateEnterPayload->SelectedAttackData->AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	EMovementRangeResult MovementRangeResult = CombatDistance::EvaluateAttackRange(Enemy, HeroTarget, AttackCDO->MinRange, AttackCDO->MaxRange);
	if (MovementRangeResult == EMovementRangeResult::InRange)
	{
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
	EvaluateAndStartMovement(MovementStateEnterPayload);
}

void UMovementState::EvaluateAndStartMovement(TSharedPtr<FMovementStatePayload> Payload)
{
	StartMovementChain(Payload->SelectedMovementChainData);
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

	EMovementRangeResult MovementRangeResult = CombatDistance::EvaluateAttackRange(Enemy, HeroTarget, SelectedAttackCDO->MinRange, SelectedAttackCDO->MaxRange);
	if (MovementRangeResult == EMovementRangeResult::TooClose) 
	{
		TryBackStep();
	}
	else if (MovementRangeResult == EMovementRangeResult::InRange) 
	{
		MovementManager->StopChain();

		UAttackDataBase* SelectedAttackData = MovementStateEnterPayload->SelectedAttackData;
		TSharedPtr<FAttackStatePayload> AttackPayload = MakeShared<FAttackStatePayload>(SelectedAttackData);

		BroadcastTransition(GAS_Tags::TAG_AI_State_Attack, AttackPayload, "Reached attack range");
	}
	else if (MovementRangeResult == EMovementRangeResult::TooFar) 
	{
		// Chain is continue for distance closing
	}
}

void UMovementState::TryBackStep()
{
	UGAS_GameplayAbilityBase* CorrectiveMovement = MovementManager->ExecuteCorrectiveMovement(StepBackMovementData);
	if (CorrectiveMovement)
	{
		CorrectiveMovement->OnAbilityEnded.RemoveAll(this);
		CorrectiveMovement->OnAbilityEnded.AddUObject(this, &UMovementState::OnBackStepEnded);
		bStepBackActive = true;
	}
}

void UMovementState::OnBackStepEnded(const FCustomAbilityEndedData& ReactionMovementEndedData)
{
	bStepBackActive = false;
}

void UMovementState::OnMovementChainEnded()
{
	EMovementRangeResult MovementRangeResult = CombatDistance::EvaluateAttackRange(Enemy, HeroTarget, SelectedAttackCDO->MinRange, SelectedAttackCDO->MaxRange);
	if (MovementRangeResult == EMovementRangeResult::InRange) 
	{
		BroadcastTransition(FGameplayTag(), nullptr, "Reached attack range");
		return;
	}

	StartMovementChain(MovementStateEnterPayload->SelectedMovementChainData);
}

void UMovementState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (MovementManager && MovementManager->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManager->OnMovementChainEnded.RemoveDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

