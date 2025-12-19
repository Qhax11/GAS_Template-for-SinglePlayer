// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Abilities/Tasks/AT_AIMoveTo.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UAT_AIMoveTo* UAT_AIMoveTo::AIMoveTo(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	AAIController* AIController,
	FVector GoalLocation,
	float AcceptanceRadius,
	float MinDuration,
	float MovementSpeed)
{
	UAT_AIMoveTo* MyTask = NewAbilityTask<UAT_AIMoveTo>(OwningAbility, TaskInstanceName);
	MyTask->CachedAIController = AIController;
	MyTask->CachedGoalLocation = GoalLocation;
	MyTask->CachedAcceptanceRadius = AcceptanceRadius;
	MyTask->CachedMinDuration = MinDuration;
	MyTask->CachedMovementSpeed = MovementSpeed;
	MyTask->bUseLocationGoal = true;

	return MyTask;
}

UAT_AIMoveTo* UAT_AIMoveTo::AIMoveToActor(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	AAIController* AIController,
	AActor* GoalActor,
	float AcceptanceRadius,
	float MinDuration,
	float MovementSpeed)
{
	UAT_AIMoveTo* MyTask = NewAbilityTask<UAT_AIMoveTo>(OwningAbility, TaskInstanceName);
	MyTask->CachedAIController = AIController;
	MyTask->CachedGoalActor = GoalActor;
	MyTask->CachedAcceptanceRadius = AcceptanceRadius;
	MyTask->CachedMinDuration = MinDuration;
	MyTask->CachedMovementSpeed = MovementSpeed;
	MyTask->bUseLocationGoal = false;

	return MyTask;
}

void UAT_AIMoveTo::Activate()
{
	Super::Activate();

	// Reset state
	bMinDurationReached = false;
	bMovementCompleted = false;
	bMovementStarted = false;

	// Validate
	if (!CachedAIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityTask_AIMoveTo: AIController is null"));
		OnFailed.Broadcast();
		EndTask();
		return;
	}

	if (!bUseLocationGoal && !CachedGoalActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityTask_AIMoveTo: Goal actor is null"));
		OnFailed.Broadcast();
		EndTask();
		return;
	}

	// Override movement speed if specified
	if (CachedMovementSpeed > 0.0f)
	{
		if (ACharacter* Character = Cast<ACharacter>(CachedAIController->GetPawn()))
		{
			if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
			{
				OriginalMaxWalkSpeed = MovementComp->MaxWalkSpeed;
				MovementComp->MaxWalkSpeed = CachedMovementSpeed;
			}
		}
	}

	// Start movement
	StartMovement();

	// Start min duration timer if specified
	if (CachedMinDuration > 0.0f && bMovementStarted)
	{
		UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(Ability, CachedMinDuration);
		WaitTask->OnFinish.AddDynamic(this, &UAT_AIMoveTo::OnMinDurationReached);
		WaitTask->ReadyForActivation();
	}
	else
	{
		// No min duration requirement
		bMinDurationReached = true;
	}
}

void UAT_AIMoveTo::StartMovement()
{
	FAIMoveRequest MoveReq;

	if (bUseLocationGoal)
	{
		MoveReq.SetGoalLocation(CachedGoalLocation);
	}
	else
	{
		MoveReq.SetGoalActor(CachedGoalActor);
	}

	MoveReq.SetAcceptanceRadius(CachedAcceptanceRadius);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);

	FNavPathSharedPtr NavPath;
	EPathFollowingRequestResult::Type MoveResult = CachedAIController->MoveTo(MoveReq, &NavPath);

	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityTask_AIMoveTo: MoveTo request failed immediately"));
		OnFailed.Broadcast();
		EndTask();
		return;
	}

	// Bind to movement completion
	UPathFollowingComponent* PathComp = CachedAIController->GetPathFollowingComponent();
	if (!PathComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityTask_AIMoveTo: PathFollowingComponent is null"));
		OnFailed.Broadcast();
		EndTask();
		return;
	}

	MoveCompletedHandle = PathComp->OnRequestFinished.AddUObject(this, &UAT_AIMoveTo::OnMoveCompleted);
	bMovementStarted = true;
}

void UAT_AIMoveTo::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	// Handle different result codes
	if (Result.Code == EPathFollowingResult::Aborted)
	{
		// Movement was cancelled (likely by another ability)
		UE_LOG(LogTemp, Log, TEXT("AbilityTask_AIMoveTo: Movement was aborted"));
		OnAborted.Broadcast();
		EndTask();
		return;
	}

	if (!Result.IsSuccess())
	{
		// Movement failed for some reason
		UE_LOG(LogTemp, Warning, TEXT("AbilityTask_AIMoveTo: Movement failed with code %d"), (int32)Result.Code);
		OnFailed.Broadcast();
		EndTask();
		return;
	}

	// Movement completed successfully
	bMovementCompleted = true;
	TryComplete();
}

void UAT_AIMoveTo::OnMinDurationReached()
{
	bMinDurationReached = true;
	TryComplete();
}

void UAT_AIMoveTo::TryComplete()
{
	// Only complete when BOTH conditions are met
	if (bMinDurationReached && bMovementCompleted)
	{
		OnCompleted.Broadcast();
		EndTask();
	}
}

void UAT_AIMoveTo::Cleanup()
{
	// Remove movement callback
	if (CachedAIController && CachedAIController->GetPathFollowingComponent() && MoveCompletedHandle.IsValid())
	{
		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.Remove(MoveCompletedHandle);
		MoveCompletedHandle.Reset();
	}

	// Restore original speed
	if (CachedMovementSpeed > 0.0f && CachedAIController)
	{
		if (ACharacter* Character = Cast<ACharacter>(CachedAIController->GetPawn()))
		{
			if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
			{
				MovementComp->MaxWalkSpeed = OriginalMaxWalkSpeed;
			}
		}
	}
}

void UAT_AIMoveTo::OnDestroy(bool bInOwnerFinished)
{
	Cleanup();
	Super::OnDestroy(bInOwnerFinished);
}