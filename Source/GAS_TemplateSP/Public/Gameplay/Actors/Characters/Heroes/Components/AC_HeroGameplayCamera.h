// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "AC_HeroGameplayCamera.generated.h"

USTRUCT(BlueprintType)
struct FCameraZoomParams
{
	GENERATED_BODY()

	// Target arm length for this zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetArmLength = 400.f;

	// How long the zoom should take
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.3f;

	// Optional curve for Behavior of arm lenght change speed (can be null)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* Curve = nullptr;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API UAC_HeroGameplayCamera : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroGameplayCamera();

	void ShakeCamera(float Force);

	// Example public calls
	void StartCameraZoomIn();
	void StartCameraZoomOut();

	// Generic zoom starter
	void StartZoom(const FCameraZoomParams& Params);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Handles the zoom logic
	void TickZoom(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_CameraZoomIn();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGAS_HeroBase* OwnerHero;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;

	UPROPERTY()
	class APlayerController* OwnerHeroPS;

	// Active zoom data
	bool bZooming = false;
	float ZoomElapsed = 0.f;
	float StartArmLength = 0.f;
	float TargetArmLength = 0.f;

	FCameraZoomParams ActiveZoomParams;

	// Configurable zoom presets
	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	FCameraZoomParams ZoomInParams;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	FCameraZoomParams ZoomOutParams;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	FCameraZoomParams AfterZoomInOutParams;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	FCameraZoomParams AfterZoomOutInParams;

};
