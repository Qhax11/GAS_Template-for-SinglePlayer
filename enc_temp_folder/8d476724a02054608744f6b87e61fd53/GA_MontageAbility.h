// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_MontageAbility.generated.h"

// Determines from which montage events the ability should end.
// Completed → End when the montage reaches the end
// BlendOut → End when the montage starts blending out
// Interrupted → End if the montage is interrupted
// Any → End on any of the above events
UENUM(BlueprintType)
enum EMontageEndPolicy : uint8
{
	// Ends ability immediately when montage finishes (no blend-out wait)
	Standard       UMETA(DisplayName = "Standard"),

	// Waits for blend-out duration before ending ability (smooth transition)
	EndWithDelay   UMETA(DisplayName = "End With Delay"),

	// Ability never ends automatically, requires manual cancellation
	Never          UMETA(DisplayName = "Never")
};

UENUM()
enum class EWarpTargetMode : uint8
{
	None           UMETA(DisplayName = "Disabled"),
	Directional    UMETA(DisplayName = "Directional"),
	TargetReach    UMETA(DisplayName = "Target Reach"),
	PreActivation  UMETA(DisplayName = "PreActivation")
};

class UAbilityTask_WaitDelay;

UCLASS()
class GAS_TEMPLATESP_API UGA_MontageAbility : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_MontageAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void EndAbilityManually() override;

	void CreatePlayMontageWaitForEvent();

	void TryActivateMotionWarping();

	bool TryBuildWarpTarget(FVector& OutLocation, FRotator& OutRotation);

	virtual FVector CalculateDirectionalWarpLocation() const;

	bool TryCalculateReachLocationToTarget(FVector& OutTargetLocation) const;

	AActor* GetCurrentTargetActor() const;

	void SetPreActivationWarpTarget(const FVector& InLocation, const FRotator& InRotation);

	bool IsWarpDistanceValid(const FVector& TargetLocation) const;

#if WITH_EDITOR
	void DebugDrawWarpTarget(const FVector& TargetLocation) const;
#endif // WITH_EDITOR

	void CleanupMotionWarping();

	void CleanupPhaseTags();

	// ****************************** ANIMATION MONTAGE ****************************** //
	class UGAS_Task_PlayMontageWaitForEvent* PlayMontageWaitForEventTask;

	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	TEnumAsByte<EMontageEndPolicy> MontageEndPolicy = EMontageEndPolicy::Standard;

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

	// ****************************** MOTION WARPING ****************************** //
	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping")
	EWarpTargetMode WarpTargetMode = EWarpTargetMode::TargetReach;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "WarpTargetMode != EWarpTargetMode::None"))
	FName MotionWarpingName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "WarpTargetMode == EWarpTargetMode::Directional"))
	float MotionWarpingDistance = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (Categories = "Gameplay.Direction", EditCondition = "WarpTargetMode == EWarpTargetMode::Directional"))
	FGameplayTag DirectionTag = GAS_Tags::TAG_Gameplay_Direction_Forward;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "WarpTargetMode == EWarpTargetMode::TargetReach"))
	float TargetReachDistance = 150.0f;

	FVector PreActivationWarpLocation = FVector::ZeroVector;

	FRotator PreActivationWarpRotation = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = "WarpTargetMode != EWarpTargetMode::None"))
	bool bDebugPointMotionWarping = false;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> BlendOutDelayTask;

	void HandleMontageEvent(bool bWasCancelled);

	UFUNCTION()
	virtual void OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);

	void WaitForBlendOutAndEnd(bool bWasCancelled);

	void CancelBlendOutDelay();

	UFUNCTION()
	void OnBlendOutDelayFinished();

	UFUNCTION()
	virtual void OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	// Handles incoming Gameplay Events signaled by Anim Notifies. 
	// This function serves as the central hub for executing animation-driven commands 
	// such as controlling the ability's lifecycle (ending the ability) and character positioning.
	UFUNCTION()
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
