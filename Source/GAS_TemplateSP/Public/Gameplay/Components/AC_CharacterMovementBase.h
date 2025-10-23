// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "AC_CharacterMovementBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_CharacterMovementBase : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0.f, const FVector& MoveDelta = FVector::ZeroVector) override;

	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;

	virtual float SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact) override;

    virtual FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;

	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;

	virtual bool ShouldCheckForValidLandingSpot(float DeltaTime, const FVector& Delta, const FHitResult& Hit) const override;

	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

	virtual void SetPostLandedPhysics(const FHitResult& Hit) override;

	virtual void FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult = NULL) const override;

	virtual bool IsWalkable(const FHitResult& Hit) const override;


	//virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	void DebugLogMovement(const FString& FunctionName, const FVector& VelocityBefore,
		const FVector& VelocityAfter, const FHitResult* Hit = nullptr);
};
