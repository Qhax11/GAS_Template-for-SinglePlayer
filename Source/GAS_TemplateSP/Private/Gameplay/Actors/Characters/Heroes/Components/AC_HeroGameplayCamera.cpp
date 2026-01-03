// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroGameplayCamera.h"
#include <Kismet/GameplayStatics.h>

UAC_HeroGameplayCamera::UAC_HeroGameplayCamera()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_HeroGameplayCamera::BeginPlay()
{
	Super::BeginPlay();

	OwnerHero = Cast<AGAS_HeroBase>(GetOwner());
	if (!OwnerHero)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerHero is null in: %s!"), *GetName());
		return;
	}

	OwnerHeroPS = OwnerHero->GetPlayerController();
	if (!OwnerHeroPS)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerHeroPS is null in: %s!"), *GetName());
		return;
	}

	CameraBoom = OwnerHero->FindComponentByClass<USpringArmComponent>();
	if (!CameraBoom)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraBoom not found in: %s!"), *GetName());
		return;
	}
}

void UAC_HeroGameplayCamera::StartCameraZoomIn()
{
	StartZoom(ZoomInParams);
}

void UAC_HeroGameplayCamera::StartCameraZoomOut()
{
	StartZoom(ZoomOutParams);
}

void UAC_HeroGameplayCamera::StartZoom(const FCameraZoomParams& Params)
{
	if (!CameraBoom)
	{
		return;
	}

	ActiveZoomParams = Params;
	ZoomElapsed = 0.f;
	StartArmLength = CameraBoom->TargetArmLength;
	TargetArmLength = Params.TargetArmLength;
	bZooming = true;
}

void UAC_HeroGameplayCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickZoom(DeltaTime);
}

void UAC_HeroGameplayCamera::TickZoom(float DeltaTime)
{
	if (!bZooming || !CameraBoom) return;

	ZoomElapsed += DeltaTime;
	float Alpha = FMath::Clamp(ZoomElapsed / ActiveZoomParams.Duration, 0.f, 1.f);

	// Use curve if defined, otherwise linear
	float CurveValue = ActiveZoomParams.Curve ? ActiveZoomParams.Curve->GetFloatValue(Alpha) : Alpha;

	CameraBoom->TargetArmLength = FMath::Lerp(StartArmLength, TargetArmLength, CurveValue);

	if (Alpha >= 1.f)
	{
		bZooming = false;
		CameraBoom->TargetArmLength = TargetArmLength;

		// 🔥 Auto chaining
		if (ActiveZoomParams.TargetArmLength == ZoomInParams.TargetArmLength)
		{
			// Zoom In bitti → otomatik Zoom Out başlat
			StartZoom(AfterZoomInOutParams);
		}
		else if (ActiveZoomParams.TargetArmLength == ZoomOutParams.TargetArmLength)
		{
			// Zoom Out bitti → otomatik Zoom In başlat
			StartZoom(AfterZoomOutInParams);
		}
	}
}

void UAC_HeroGameplayCamera::ShakeCamera(float Force)
{
	if (!OwnerHeroPS)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerHeroPS is null in: %s!"), *GetName());
		return;
	}

	OwnerHeroPS->ClientStartCameraShake(CameraShakeClass, Force);
}

