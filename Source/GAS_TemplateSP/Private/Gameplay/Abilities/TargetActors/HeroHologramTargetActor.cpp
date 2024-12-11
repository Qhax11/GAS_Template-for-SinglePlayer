// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Kismet/KismetMathLibrary.h"

void AHeroHologramTargetActor::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetInstigator());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	EyeOfViewComponent = HeroBase->GetEyeOfViewComponent();
	if (!EyeOfViewComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EyeOfViewComponent is null in: %s"), *GetName());
		return;
	}

	TargetLockSystemComponent = HeroBase->GetTargetLockSystemComponent();
	if (!TargetLockSystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLockSystemComponent is null in: %s"), *GetName());
		return;
	}
	TargetLockSystemComponent->OnTargetChanged.AddDynamic(this, &AHeroHologramTargetActor::OnTargetChaned);
	TargetLockSystemComponent->OnEndTargetLock.AddDynamic(this, &AHeroHologramTargetActor::OnEndTargetLock);
	CurrentTarget = TargetLockSystemComponent->CurrentTarget;
}

void AHeroHologramTargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentTarget) 
	{
		RotateToTarget(CurrentTarget, DeltaSeconds);
	}
	else 
	{
		SyncRotationWithHero(DeltaSeconds);
	}

}

void AHeroHologramTargetActor::Confirm()
{
	OnConfirm.Broadcast(FGAS_TargetActorData(this, nullptr));
}

void AHeroHologramTargetActor::Cancel()
{
	OnCancel.Broadcast(FGAS_TargetActorData(this, nullptr));
}

void AHeroHologramTargetActor::OnTargetChaned(AActor* NewTarget)
{
	BP_OnTargetChaned(NewTarget);
}

void AHeroHologramTargetActor::OnEndTargetLock()
{
	BP_OnEndTargetLock();
}

void AHeroHologramTargetActor::RotateToTarget(AActor* TargetActor, float DeltaTime)
{
	if (!TargetActor) 
	{
		return;
	} 

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll));
}

void AHeroHologramTargetActor::SyncRotationWithHero(float DeltaTime)
{
	if (!HeroBase)
	{
		return;
	}

	FRotator CurrentRotation = GetActorRotation();
	FRotator HeroRotation = HeroBase->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, HeroRotation, DeltaTime, RotationSpeed);

	SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll));
}
