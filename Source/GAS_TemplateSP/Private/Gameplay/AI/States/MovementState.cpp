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

	TSharedPtr<FMovementStatePayload> MovementStatePayload = StaticCastSharedPtr<FMovementStatePayload>(EnterPayload);
	if (!MovementStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackStatePayload is invalid in: %s"), *GetName());
		return;
	}

	SelectedAttackCDO = MovementStatePayload->TargetAttackClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	StartMovementChain(MovementStatePayload);
}

void UMovementState::OnTick_Implementation(float DeltaTime)
{
	TryEnterToAttackState();
}

void UMovementState::TryEnterToAttackState()
{
	if (!SelectedAttackCDO)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	if (IsInRangeForAttack()) 
	{
		MovementManagerComponent->StopMovementAbilities();
		ExitRequest("Target is in range", GAS_Tags::TAG_AI_State_Attack);
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
		UE_LOG(LogTemp, Warning, TEXT("Ability Class is null in: %s"), *GetName());
		return false;
	}

	float Distance = EnemyController->GetTargetHeroDistance();
	return (SelectedAttackCDO->MaxRange > Distance && SelectedAttackCDO->MinRange < Distance);
}

void UMovementState::StartMovementChain(TSharedPtr<FMovementStatePayload> MovementStatePayload)
{
	if (!MovementStatePayload.IsValid() || !MovementStatePayload->MovementChainAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovementStatePayload is invalid in: %s"), *GetName());
		return;
	}

	/*
	MovementManagerComponent->StartMovementChain(SelectedAttackAbilityClass);
	*/
	if (!MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded)) 
	{
		MovementManagerComponent->OnMovementChainEnded.AddDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}

void UMovementState::OnMovementChainEnded()
{
	ExitRequest("MovementChain is ended", GAS_Tags::TAG_AI_State_Movement);
}

void UMovementState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (MovementManagerComponent && MovementManagerComponent->OnMovementChainEnded.IsAlreadyBound(this, &UMovementState::OnMovementChainEnded))
	{
		MovementManagerComponent->OnMovementChainEnded.RemoveDynamic(this, &UMovementState::OnMovementChainEnded);
	}
}