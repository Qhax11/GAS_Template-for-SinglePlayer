// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h" 
#include "SC_HeroHologramController.generated.h"

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
	void SetHeroHologramLocation();

	UFUNCTION(BlueprintCallable, Category = "CalculateHologramTargetActorLocation")
	FVector GetHeroHologramLocationFromLineTrace();

	UFUNCTION(BlueprintCallable, Category = "CalculateHologramTargetActorLocation")
	FVector GetHeroHologramLocationFromLineTraceTargetLocked();

public:
	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	UCurveFloat* C_MouseInoutSensitiveX;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	float SensitiveMultiplierX = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "HologramAbilityHelper|Config")
	UCurveFloat* C_MouseInoutSensitiveY;
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

private:

	FVector2D CalculateCumulativeMouseInputs();
	float CumulativeMouseDeltaX;
	float CumulativeMouseDeltaY;

	UFUNCTION()
	void OnStartTargetLock();
	float GetPointDistToLine();
	float CalculateTraceForwardDistanceOffset();

	UFUNCTION()
	void OnEndTargetLock();

	void UpdateTraceForwardDistance();
	void UpdateTraceRightDistance();

	void LookAtTarget();

	bool bTargetLocked = false;
};
