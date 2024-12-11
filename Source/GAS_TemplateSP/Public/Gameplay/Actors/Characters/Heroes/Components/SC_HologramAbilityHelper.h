// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h" 
#include "SC_HologramAbilityHelper.generated.h"

/**
 * USC_HologramAbilityHelper
 *
 * This component is designed to mimic the rotation of the player's camera controlled by mouse inputs.
 * It is particularly useful in scenarios where the camera's viewpoint is locked or fixed, but an associated
 * actor or component needs to replicate the same rotational behavior as the camera.
 *
 * Ideal for abilities such as hologram-based effects or visual representations that need to stay
 * synchronized with the camera's orientation.
 */


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API USC_HologramAbilityHelper : public USceneComponent
{
	GENERATED_BODY()

public:
	USC_HologramAbilityHelper();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateRotationFromMouseInput(const FVector2D& MouseInput);

	UFUNCTION(BlueprintCallable, Category = "CalculateHologramTargetActorLocation")
	FVector CalculateHologramTargetActorLocation(bool bDrawDebug);

	UPROPERTY(EditDefaultsOnly, Category = "CalculateHologramTargetActorLocation")
	UCurveFloat* TraceDistanceCurve;

	UPROPERTY(BlueprintReadOnly, Category = "CalculateHologramTargetActorLocation")
	float TraceDistance = 0.f;

private:
	class APlayerController* PC;

	class AGAS_HeroBase* HeroBase;

};
