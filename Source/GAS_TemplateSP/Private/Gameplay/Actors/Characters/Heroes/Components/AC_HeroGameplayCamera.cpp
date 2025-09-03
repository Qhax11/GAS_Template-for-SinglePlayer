// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroGameplayCamera.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/SpringArmComponent.h"
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
	BP_CameraZoomIn();

	/*
	if (!CameraBoom)
	{
		return;
	}

	bZooming = true;
	ZoomElapsed = 0.f;
	StartArmLength = CameraBoom->TargetArmLength;
	TargetArm = 400;
	bZooming = true;
	*/
}

void UAC_HeroGameplayCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CameraZoomIn(DeltaTime);
}

void UAC_HeroGameplayCamera::CameraZoomIn(float DeltaTime)
{
	if (!bZooming || !CameraBoom)
	{
		return;
	}

	ZoomElapsed += DeltaTime;
	float Alpha = FMath::Clamp(ZoomElapsed / ZoomDuration, 0.f, 1.f);

	// Curve varsa curve deðeri al, yoksa lineer
	if (ZoomCurve)
	{
		Alpha = ZoomCurve->GetFloatValue(Alpha);
	}

	// SpringArm target length interpolasyonu
	CameraBoom->TargetArmLength = FMath::Lerp(StartArmLength, TargetArm, Alpha);

	if (ZoomElapsed >= ZoomDuration)
	{
		bZooming = false;
		CameraBoom->TargetArmLength = TargetArm; // bitiþ deðerini set et
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

