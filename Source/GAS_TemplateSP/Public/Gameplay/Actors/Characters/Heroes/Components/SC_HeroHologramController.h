// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h" 
#include "SC_HeroHologramController.generated.h"

/**
 * USC_HeroHologramController
 *
 * This component is responsible for managing and controlling the behavior of the hologram in the game.
 * It provides functionality to set the hologram's location, handle detailed calculations for precise placement,
 * and synchronize with gameplay elements such as mouse inputs or target locking.
 *
 * Key features include:
 * - Setting and retrieving the hologram's location dynamically using line traces.
 * - Handling mouse input sensitivity for smooth and responsive control.
 * - Maintaining consistent trace lengths and offsets using geometric calculations.
 * - Debugging support to visualize trace paths and adjustments.
 *
 * Ideal for gameplay systems where holograms need to interact seamlessly with the player's input
 * and world environment, ensuring accurate alignment and placement.
 */


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API USC_HeroHologramController : public USceneComponent
{
	GENERATED_BODY()

public:
	USC_HeroHologramController();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class APlayerController* PC;
	class AGAS_HeroBase* HeroBase;
	class UAC_TargetLockSystem* TargetLockSystem;

public:
	void LookAtTarget();

	void SetHeroHologramLocation();

	UFUNCTION(BlueprintCallable, Category = "CalculateHologramTargetActorLocation")
	FVector GetHeroHologramLocationFromLineTrace();

	UFUNCTION(BlueprintCallable, Category = "CalculateHologramTargetActorLocation")
	FVector GetHeroHologramLocationFromLineTraceTargetLocked();

public:
	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	UCurveFloat* C_TraceRightDistance;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config", Meta = (ToolTip = "Reverse curve of C_TraceRightDistanceReverse"))
	UCurveFloat* C_TraceRightDistanceReverse;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	float SensitiveMultiplierX = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	UCurveFloat* C_TraceForwardDistance;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config", Meta = (ToolTip = "Reverse curve of C_TraceForwardDistance"))
	UCurveFloat* C_TraceForwardDistanceReverse;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	float SensitiveMultiplierY = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	bool bDrawDebug;

public:
	TObjectPtr<class AHeroHologramTargetActor> HeroHologramTargetActor = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceForwardDistance = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceForwardDistanceOffset = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceRightDistance = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Trace")
	float TraceRightDistanceOffset = 0.f;

public:
	FVector2D CalculateCumulativeMouseInputs();

	void ResetRightTraceDistance();

	// Setting location with CumulativeMouseValues
	void SetCumulativeMouseValuesRelatedWith2DLocation(FVector2D Location);

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Input")
	float CumulativeMouseDeltaX;

	UPROPERTY(BlueprintReadOnly, Category = "HologramAbilityHelper|Input")
	float CumulativeMouseDeltaY;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Input", Meta = (ToolTip = "The maximum cumulative mouse delta Y value that feeds into the forward distance value"))
	float MaxCumulativeMouseDeltaY = 0;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Input", Meta = (ToolTip = "The minimum cumulative mouse delta Y value that feeds into the forward distance value"))
	float MinCumulativeMouseDeltaY = -800;

private:
	UFUNCTION()
	void OnStartTargetLock();
	float GetPointDistToLine();

	// Calculates the forward distance offset using the Pythagorean theorem. 
    // Ensures that the total trace length stays constant even when there is an offset in the right direction or the trace direction changes.
	float CalculateTraceForwardDistanceOffset();

	UFUNCTION()
	void OnEndTargetLock();

	void UpdateTraceForwardDistance();

	void UpdateTraceRightDistance();

	void SetCumulativeMouseDeltaXForRightTraceDistaneValue(float Value);

	void SetCumulativeMouseDeltaYForForwardTraceDistaneValue(float Value);

	float GetTimeForTraceFowardDistance(float Value);
	float GetTimeForTraceRightDistance(float Value);

	bool bTargetLocked = false;
};
