// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
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
