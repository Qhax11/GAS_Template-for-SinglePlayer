// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GameFramework/RootMotionSource.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "GA_DashBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_DashBase : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	UGA_DashBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	void BindRootMotionTask();

	virtual FVector CalculateDestination();

	UAbilityTask_ApplyRootMotionMoveToForce* DashRootMotionTask;

	UFUNCTION()
	void OnTaskTimedOut();

	UFUNCTION()
	void OnTimedOutAndDestinationReached();

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	float DistanceMultiplier = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	float Duration = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	bool bSetNewMovementMode;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	TEnumAsByte<EMovementMode> NewMovementMode = EMovementMode::MOVE_Walking;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	bool bRestrictSpeedToExpected = false;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	class UCurveVector* DashCurve;

	// What to do with character's Velocity when root motion finishes 
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	ERootMotionFinishVelocityMode FinishVelocityMode = ERootMotionFinishVelocityMode::ClampVelocity;

	// If FinishVelocityMode mode is "SetVelocity", character velocity is set to this value when root motion finishes 
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	FVector FinishSetVelocity = FVector(0, 0, 0);

	// If FinishVelocityMode mode is "ClampVelocity", character velocity is clamped to this value when root motion finishes 
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	float FinishClampVelocity = 0.f;
};
