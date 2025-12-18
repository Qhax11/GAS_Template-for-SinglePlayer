// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainDataa.h"
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
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: MovementStateEnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	if (!MovementStateEnterPayload->SelectedAttackData->AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: SelectedAttackData->AbilityClass is invalid in: %s"), *GetName());
		return false;
	}

	if (!IsValid(Enemy) || !IsValid(HeroTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: Enemy or HeroTarget is invalid in: %s"), *GetName());
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

void UMovementState::StartMovementChain(UMovementChainDataa* MovementChainData)
{
	if (!MovementChainData || !MovementManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: MovementChainData or MovementManager is invalid in: %s"), *GetName());
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
	if (!SelectedAttackCDO)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	if (IsReachedAttackRange())
	{
		MovementManager->StopMovementAbilities();

		UAttackDataBase* SelectedAttackData = MovementStateEnterPayload->SelectedAttackData;
		TSharedPtr<FAttackStatePayload> AttackPayload = MakeShared<FAttackStatePayload>(SelectedAttackData);

		FStateTransitionRequest StateTransitionRequest = FStateTransitionRequest(GAS_Tags::TAG_AI_State_Attack, AttackPayload);
		ExitRequest("Target is in range", StateTransitionRequest);
	}
}

bool UMovementState::IsReachedAttackRange() const
{
	if (!SelectedAttackCDO || !IsValid(Enemy) || !IsValid(HeroTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UMovementState: SelectedAttackCDO or EnemyController is null in %s"), *GetName());
		return false;
	}

	const float MinRange = FMath::Max(0.f, SelectedAttackCDO->MinRange);
	const float MaxRange = SelectedAttackCDO->MaxRange;

	return CombatDistance::IsInRange(Enemy, HeroTarget, MaxRange) && !CombatDistance::IsInRange(Enemy, HeroTarget, MinRange);
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

