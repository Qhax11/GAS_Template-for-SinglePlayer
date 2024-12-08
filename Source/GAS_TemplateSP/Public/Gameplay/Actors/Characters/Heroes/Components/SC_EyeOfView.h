// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SC_EyeOfView.generated.h"

/**
 * USC_EyeOfView
 *
 * This component is designed to mimic the rotation of the player's camera controlled by mouse inputs.
 * It is particularly useful in scenarios where the camera's viewpoint is locked or fixed, but an associated
 * actor or component needs to replicate the same rotational behavior as the camera.
 *
 * Ideal for abilities such as hologram-based effects or visual representations that need to stay
 * synchronized with the camera's orientation.
 */


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API USC_EyeOfView : public USceneComponent
{
	GENERATED_BODY()

public:
	USC_EyeOfView();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateRotationFromMouseInput(const FVector2D& MouseInput);

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 2.5f;

private:

	class APlayerController* PC;

	class AGAS_HeroBase* HeroBase;
};
