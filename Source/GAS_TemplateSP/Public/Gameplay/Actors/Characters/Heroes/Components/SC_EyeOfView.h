// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "SC_EyeOfView.generated.h"

/**
 * EyeOfView Component
 *
 * This component is designed to mimic the rotation of the camera controlled by mouse inputs.
 * It is particularly useful for abilities such as hologram-based effects, where a visual representation
 * synced to the camera's rotation is required.
 *
 * A Static Mesh is included to serve as a symbolic or visual representation of the camera's orientation.
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

	APlayerController* PC;
};
