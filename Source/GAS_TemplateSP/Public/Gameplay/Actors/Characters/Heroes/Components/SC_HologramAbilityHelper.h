// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h" 
#include "SC_HologramAbilityHelper.generated.h"

/**
 * USC_HologramAbilityHelper 
 *
 * This component is designed to replicate the rotation behavior of the player's camera based on mouse inputs,
 * specifically for hologram-based abilities. It synchronizes the rotation of the associated actor or component
 * with the camera's viewpoint, making it ideal for visual effects that need to stay aligned with the camera's orientation.
 *
 * Perfect for abilities like holograms or visual representations that must mimic the camera's rotation to maintain
 * consistent alignment during gameplay.
 */


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API USC_HologramAbilityHelper : public USceneComponent
{
	GENERATED_BODY()

public:
	USC_HologramAbilityHelper();

protected:
	virtual void BeginPlay() override;

	class UAC_TargetLockSystem* TargetLockSystem;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "CalculateHologramTargetActorLocation")
	FVector PerformLineTraceNonTargetLocked();

	UFUNCTION(BlueprintCallable, Category = "CalculateHologramTargetActorLocation")
	FVector PerformLineTraceTargetLocked();

	UPROPERTY(EditDefaultsOnly, Category = "CalculateHologramTargetActorLocation")
	UCurveFloat* TraceDistanceCurve;

public:

	TObjectPtr<class AHeroHologramTargetActor> HeroHologramTargetActor;

	UFUNCTION()
	void OnStartTargetLock();

	UFUNCTION()
	void OnEndTargetLock();



	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|MouseInputConfig")
	float SensitiveMultiplierX = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|MouseInputConfig")
	float SensitiveMultiplierY = 100.0f;



	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceForwardDistance = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceForwardDistanceOffset = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceRightDistance = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceRightDistanceOffset = 0.f;


	void UpdateTraceForwardDistance();

	void UpdateTraceRightDistance();

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Trace")
	bool bDrawDebug;

	bool bTargetLocked = false;

private:
	class APlayerController* PC;
	class AGAS_HeroBase* HeroBase;

	FVector2D CalculateCumulativeMouseInputs();
	float CumulativeMouseDeltaX;
	float CumulativeMouseDeltaY;

	void LookAtTarget();

	float GetPointDistToLine();
};
