// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroGameplayCamera.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include <Kismet/GameplayStatics.h>

UAC_HeroGameplayCamera::UAC_HeroGameplayCamera()
{
	PrimaryComponentTick.bCanEverTick = false;
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
