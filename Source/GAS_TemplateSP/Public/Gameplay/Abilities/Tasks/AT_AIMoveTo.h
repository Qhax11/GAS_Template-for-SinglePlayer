// Qhax's GAS Template for SinglePlayer
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "AT_AIMoveTo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveToTaskDelegate);

/**
 * Generic AI Movement Task with optional minimum duration
 * Can be used by any movement ability that needs AI pathfinding
 */
UCLASS()
class GAS_TEMPLATESP_API UAT_AIMoveTo : public UAbilityTask
{
	GENERATED_BODY()

public:
	/** Called when movement completes successfully (both movement finished AND min duration passed) */
	UPROPERTY(BlueprintAssignable)
	FMoveToTaskDelegate OnCompleted;

	/** Called when movement is aborted (cancelled by another ability) */
	UPROPERTY(BlueprintAssignable)
	FMoveToTaskDelegate OnAborted;

	/** Called when movement fails (pathfinding failed, invalid parameters, etc) */
	UPROPERTY(BlueprintAssignable)
	FMoveToTaskDelegate OnFailed;

	UPROPERTY(BlueprintAssignable)
	FMoveToTaskDelegate ExpectedDurationReached;

	/** Fired when minimum duration has elapsed  */
	UPROPERTY(BlueprintAssignable)
	FMoveToTaskDelegate MinDurationReached;

	/** Fired when maximum duration has elapsed  */
	UPROPERTY(BlueprintAssignable)
	FMoveToTaskDelegate MaxDurationReached;

	/**
	 * Move AI to a specific location with optional minimum duration
	 * @param OwningAbility - The ability that owns this task
	 * @param TaskInstanceName - Debug name for this task instance
	 * @param AIController - The AI controller to move
	 * @param GoalLocation - Target location to move to
	 * @param AcceptanceRadius - How close to get to the target
	 * @param MinDuration - Minimum time this movement should take
	 * @param MaxDuration - Maximum time this movement should take
	 * @param MovementSpeed - Speed override (0 = use character's current speed)
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "AI Move To Location",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_AIMoveTo* AIMoveTo(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		class AAIController* AIController,
		FVector GoalLocation,
		float AcceptanceRadius = 50.0f,
		float MinDuration = 0.25f,
		float MaxDuration = 8.0f,
		float MovementSpeed = 0.0f
	);

	/**
	 * Move AI to a target actor with optional minimum duration
	 * @param OwningAbility - The ability that owns this task
	 * @param TaskInstanceName - Debug name for this task instance
	 * @param AIController - The AI controller to move
	 * @param GoalActor - Target actor to move to
	 * @param AcceptanceRadius - How close to get to the target
	 * @param MinDuration - Minimum time this movement should take 
	 * @param MaxDuration - Maximum time this movement should take 
	 * @param MovementSpeed - Speed override (0 = use character's current speed)
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "AI Move To Actor", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_AIMoveTo* AIMoveToActor(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		class AAIController* AIController,
		AActor* GoalActor,
		float AcceptanceRadius = 50.0f,
		float MinDuration = 0.25f,
		float MaxDuration = 8.0f,
		float MovementSpeed = 0.0f
	);

	void SetExpectedDuration(float InExpectedDuration)
	{
		CachedExpectedDuration = InExpectedDuration;
	}

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void NormalizeDurations();

	// Override movement speed if specified
	void TryToSetMovementSpeed();

	void StartMovement();

	void ActivateWaitDelays();

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void TryComplete();

	UFUNCTION()
	void OnExpectedDurationReached();

	UFUNCTION()
	void OnMinDurationReached();

	UFUNCTION()
	void OnMaxDurationReached();

	void Cleanup();

	// Movement parameters
	UPROPERTY()
	TObjectPtr<class AAIController> CachedAIController;

	UPROPERTY()
	TObjectPtr<AActor> CachedGoalActor;

	bool bMinDurationReached = false;
	bool bMovementCompleted = false;

	FVector CachedGoalLocation;
	float CachedAcceptanceRadius;
	float CachedMinDuration;
	float CachedExpectedDuration;
	float CachedMaxDuration;
	float CachedMovementSpeed;

	bool bUseLocationGoal; // true = location, false = actor

	// Cleanup handles
	FDelegateHandle MoveCompletedHandle;

	// Original speed (to restore on cleanup)
	float OriginalMaxWalkSpeed;
};