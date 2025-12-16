// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/CrowdEnemy/CrowdEnemy_MovementState.h"
#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"

void UCrowdEnemy_MovementState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);

	AICrowdEventManager = GetWorld()->GetGameInstance()->GetSubsystem<US_AICrowdEventManager>();
	checkf(AICrowdEventManager, TEXT("AICrowdEventManager null in %s"), *GetClass()->GetName());
}

void UCrowdEnemy_MovementState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);
}

void UCrowdEnemy_MovementState::ExecuteMovement(TSharedPtr<FMovementStatePayload> MovementStatePayload)
{
	if (!MovementStatePayload.IsValid() || !MovementStatePayload->MovementChainAsset || !AICrowdEventManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("AICrowdEventManager is null in: %s"), *GetName());
		return;
	}

	bool IsAttackIntender = AICrowdEventManager->RequestToBeAttackIntender(EnemyASC);
	if (IsAttackIntender)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartMovementChain"));
		StartMovementChain(MovementStatePayload->MovementChainAsset);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MakeStrafingAbility"));
		MakeStrafingAbility();
	}
}

void UCrowdEnemy_MovementState::MakeStrafingAbility()
{
	UGAS_GameplayAbilityBase* ActivatedStrafingAbility = EnemyASC->TryActivateAbilityByClassWithEventData(StrafingAbilityClass, StrafingAbilityEventData);
	if (ActivatedStrafingAbility)
	{
		ActivatedStrafingAbility->OnAbilityEnded.RemoveAll(this);
		ActivatedStrafingAbility->OnAbilityEnded.AddUObject(this, &UCrowdEnemy_MovementState::OnStrafingAbilityEnded);
		LastUsedStrafingAbility = ActivatedStrafingAbility;
	}
}

void UCrowdEnemy_MovementState::OnStrafingAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	// Delay before make strafing again
	float WaitTime = FMath::RandRange(MinStrafingWaitTime, MaxStrafingWaitTime);
	GetWorld()->GetTimerManager().SetTimer(WaitForNextStrafingOrbitTimerHandle, this, &UCrowdEnemy_MovementState::OnWaitTimeFinished, WaitTime, false);
}

void UCrowdEnemy_MovementState::OnWaitTimeFinished()
{
	ExitRequest("OnWaitTimeFinished");
}

void UCrowdEnemy_MovementState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (MovementManager && MovementManager->OnMovementChainEnded.IsAlreadyBound(this, &UCrowdEnemy_MovementState::OnMovementChainEnded))
	{
		MovementManager->OnMovementChainEnded.RemoveDynamic(this, &UCrowdEnemy_MovementState::OnMovementChainEnded);
	}

	if (LastUsedStrafingAbility && LastUsedStrafingAbility->IsValidLowLevel())
	{
		LastUsedStrafingAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedStrafingAbility = nullptr;
	}

	GetWorld()->GetTimerManager().ClearTimer(WaitForNextStrafingOrbitTimerHandle);
}
