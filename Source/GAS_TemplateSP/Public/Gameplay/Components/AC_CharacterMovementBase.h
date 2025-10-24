// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "AC_CharacterMovementBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_CharacterMovementBase : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual float SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact) override;

    virtual FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;

	virtual bool ShouldCheckForValidLandingSpot(float DeltaTime, const FVector& Delta, const FHitResult& Hit) const override;

	virtual void FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult = NULL) const override;

	void DebugLogMovement(const FString& FunctionName, const FVector& VelocityBefore,
		const FVector& VelocityAfter, const FHitResult* Hit = nullptr);

	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovementBase")
	float MinDownwardComponent = -0.3f;

};
