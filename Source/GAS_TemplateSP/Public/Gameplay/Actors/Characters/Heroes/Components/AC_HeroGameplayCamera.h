// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_HeroGameplayCamera.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API UAC_HeroGameplayCamera : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroGameplayCamera();

	void ShakeCamera(float Force);

	UFUNCTION(BlueprintCallable)
	void StartCameraZoomIn();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CameraZoomIn(float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AGAS_HeroBase* OwnerHero;

	UPROPERTY()
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	class APlayerController* OwnerHeroPS;

	bool bZooming = false;
	float ZoomDuration = 0.f;
	float ZoomElapsed = 0.f;
	float StartArmLength = 0.f;
	float TargetArm = 0.f;

	UPROPERTY(EditDefaultsOnly);
	UCurveFloat* ZoomCurve = nullptr;
};
