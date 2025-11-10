// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToActorForce.h"
#include "AT_MoveToActorForce_IgnoreZ.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAT_MoveToActorForce_IgnoreZ : public UAbilityTask_ApplyRootMotionMoveToActorForce
{
	GENERATED_BODY()

public:
	/** Apply force to character's movement to move to a target actor (Ignores Z axis) */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_MoveToActorForce_IgnoreZ* ApplyRootMotionMoveToActorForce_IgnoreZ(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		AActor* InTargetActor,
		FVector InTargetLocationOffset,
		ERootMotionMoveToActorTargetOffsetType InOffsetAlignment,
		float InDuration,
		UCurveFloat* InTargetLerpSpeedHorizontal,
		UCurveFloat* InTargetLerpSpeedVertical,
		bool bInSetNewMovementMode,
		EMovementMode InMovementMode,
		bool bInRestrictSpeedToExpected,
		UCurveVector* InPathOffsetCurve,
		UCurveFloat* InTimeMappingCurve,
		ERootMotionFinishVelocityMode InVelocityOnFinishMode,
		FVector InSetVelocityOnFinish,
		float InClampVelocityOnFinish,
		bool bInDisableDestinationReachedInterrupt,
		float InReachedDestinationDistance = 50.0f);

protected:
	virtual void SharedInitAndApply() override;
	virtual void TickTask(float DeltaTime) override;

	/** Z eksenini ignore eden target offset hesaplama */
	FVector CalculateTargetOffset_IgnoreZ() const;

	/** Ýlk hedef Z pozisyonu */
	float CachedTargetZ;

	/** Z cache'lenmiþ mi? */
	bool bHasCachedZ;
};
