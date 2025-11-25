// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_MontageAbility.generated.h"

UENUM(BlueprintType)
enum EMontageEndPolicy : uint8
{
	// The ability will end if it is forcibly Interrupted OR Cancelled. 
	// This policy serves as the primary safety net, allowing forced termination.
	InterruptedOrCancelled,

	// The ability will ONLY end upon receiving the successful Finished Trigger Event. 
	// All external forced terminations (Interrupt/Cancel) are ignored. (The strict success check).
	TriggerOnly,

	// The ability will end on ANY termination event: Interrupt, Cancel, OR the Finished Trigger Event. 
	// This is the most comprehensive safety policy for combat abilities.
	Any,

	// The ability will NEVER end autonomously via any event (Interrupt, Cancel, or Finished Trigger). 
	// It must be terminated manually by external code (e.g., ClearAbility call).
	Never
};


UCLASS()
class GAS_TEMPLATESP_API UGA_MontageAbility : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_MontageAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	void ActivateMotionWarping();

	FVector CalculateDestinationReachLocation() const;

	virtual FVector CalculateMotionWarpingLocation() const;

	void CleanupMotionWarping();

	void CreatePlayMontageWaitForEvent();

	class UGAS_Task_PlayMontageWaitForEvent* PlayMontageWaitForEventTask;

	// Determines from which montage events the ability should end.
    // Completed → End when the montage reaches the end
    // BlendOut → End when the montage starts blending out
    // Interrupted → End if the montage is interrupted
    // Any → End on any of the above events
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	TEnumAsByte<EMontageEndPolicy> MontageEndPolicy = EMontageEndPolicy::Any;

	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	FName SectionName = NAME_None;

	/** Change to play the montage faster or slower */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	float PlayRate = 1.f;

	/** Any gameplay events matching this tag will activate the OnEventReceived callback and apply the gameplay effect containers for this ability */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	FGameplayTagContainer WaitForEventTag;

	/** If we need to turn off characters rotation during the gameplay effect */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	bool bTurnOffRotation;

	/** If we want that montage doesen't stop after ability end */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	bool bStopWhenAbilityEnds = true;

	// ****************************** MOTION WARPING ****************************** //

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping")
	bool bEnableMotionWarping = false;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "bEnableMotionWarping"))
	FName MotionWarpingName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "bEnableMotionWarping && !bUseDestinationReachForDistance"))
	float MotionWarpingDistance = 0.0f;

	/**
    * If true, the ability will calculate a motion warp target based on the distance to the destination.
    * This is generally used for AI characters to stop a fixed distance away from their target
    * rather than moving a fixed forward/backward distance.
    * Essentially, it allows AI to “approach but not overshoot” the target.
    */
	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "bEnableMotionWarping"))
	bool bUseDestinationReachForDistance = false;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "bEnableMotionWarping && bUseDestinationReachForDistance"))
	float DestinationReachDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "bEnableMotionWarping"), meta = (Categories = "Gameplay.Direction"))
	FGameplayTag DirectionTag = GAS_Tags::TAG_Gameplay_Direction_Forward;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "bEnableMotionWarping"))
	bool bDebugPointMotionWarping = false;

protected:
	UFUNCTION()
	virtual void OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	// Handles incoming Gameplay Events signaled by Anim Notifies. 
	// This function serves as the central hub for executing animation-driven commands 
	// such as controlling the ability's lifecycle (ending the ability) and character positioning.
	UFUNCTION()
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
