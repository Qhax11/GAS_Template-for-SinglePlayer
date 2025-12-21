// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Abilities/Tasks/AT_AIMoveTo.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAT_AIMoveTo* UAT_AIMoveTo::AIMoveToLocation(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	AAIController* AIController,
	FVector GoalLocation,
	float AcceptanceRadius,
	float ExpectedDuration,
	float MinDuration,
	float MaxDuration,
	float MovementSpeed)
{
	UAT_AIMoveTo* MyTask = NewAbilityTask<UAT_AIMoveTo>(OwningAbility, TaskInstanceName);
	MyTask->CachedAIController = AIController;
	MyTask->CachedGoalLocation = GoalLocation;
	MyTask->CachedAcceptanceRadius = AcceptanceRadius;
	MyTask->CachedExpectedDuration = ExpectedDuration;
	MyTask->CachedMinDuration = MinDuration;
	MyTask->CachedMaxDuration = MaxDuration,
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
	float ExpectedDuration,
	float MinDuration,
	float MaxDuration,
	float MovementSpeed)
{
	UAT_AIMoveTo* MyTask = NewAbilityTask<UAT_AIMoveTo>(OwningAbility, TaskInstanceName);
	MyTask->CachedAIController = AIController;
	MyTask->CachedGoalActor = GoalActor;
	MyTask->CachedAcceptanceRadius = AcceptanceRadius;
	MyTask->CachedExpectedDuration = ExpectedDuration;
	MyTask->CachedMinDuration = MinDuration;
	MyTask->CachedMaxDuration = MaxDuration;
	MyTask->CachedMovementSpeed = MovementSpeed;
	MyTask->bUseLocationGoal = false;
	return MyTask;
}

void UAT_AIMoveTo::Activate()
{
	Super::Activate();

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

	NormalizeDurations();
	TryToSetMovementSpeed();
	StartMovement();
	ActivateWaitDelays();
}

void UAT_AIMoveTo::NormalizeDurations()
{
	// Min / Max mechanical constraints
	CachedMinDuration = FMath::Max(0.0f, CachedMinDuration);
	CachedMaxDuration = FMath::Max(0.0f, CachedMaxDuration);

	// Hard safety cap for Max (task protection, NOT design)
	static constexpr float MAX_TASK_DURATION = 60.0f;
	if (CachedMaxDuration > MAX_TASK_DURATION)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveTo: MaxDuration clamped to %f"), MAX_TASK_DURATION);
		CachedMaxDuration = MAX_TASK_DURATION;
	}

	// Logical invariant
	if (CachedMaxDuration > 0.0f && CachedMinDuration > CachedMaxDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveTo: MinDuration > MaxDuration, clamping Min to Max"));
		CachedMinDuration = CachedMaxDuration;
	}

	// ExpectedDuration: DO NOT clamp, just warn
	if (CachedExpectedDuration > 0.0f && CachedMaxDuration > 0.0f && CachedExpectedDuration > CachedMaxDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveTo: ExpectedDuration > MaxDuration (policy issue)"));
	}
}

void UAT_AIMoveTo::TryToSetMovementSpeed()
{
	if (CachedMovementSpeed <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveTo: MovementSpeed <= 0, using character default speed"));
		return;
	}

	if (ACharacter* Character = Cast<ACharacter>(CachedAIController->GetPawn()))
	{
		if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
		{
			OriginalMaxWalkSpeed = MovementComp->MaxWalkSpeed;
			MovementComp->MaxWalkSpeed = CachedMovementSpeed;
		}
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
}

void UAT_AIMoveTo::ActivateWaitDelays()
{
	if (CachedExpectedDuration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ExpectedDurationTimer,
			this,
			&UAT_AIMoveTo::OnExpectedDurationReached,
			CachedExpectedDuration,
			false
		);
	}

	if (CachedMinDuration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			MinDurationTimer,
			this,
			&UAT_AIMoveTo::OnMinDurationReached,
			CachedMinDuration,
			false
		);
	}

	if (CachedMaxDuration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			MaxDurationTimer,
			this,
			&UAT_AIMoveTo::OnMaxDurationReached,
			CachedMaxDuration,
			false
		);
	}
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

	// ?? movement gerçekten bitti
	bMovementCompleted = true;
	TryComplete();
}

void UAT_AIMoveTo::TryComplete()
{
	if (bMovementCompleted && bMinDurationReached)
	{
		OnCompleted.Broadcast();
		EndTask();
	}
}

void UAT_AIMoveTo::OnExpectedDurationReached()
{
	ExpectedDurationReached.Broadcast();
}

void UAT_AIMoveTo::OnMinDurationReached()
{
	bMinDurationReached = true;
	MinDurationReached.Broadcast();
	TryComplete();
}

void UAT_AIMoveTo::OnMaxDurationReached()
{
	MaxDurationReached.Broadcast();
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