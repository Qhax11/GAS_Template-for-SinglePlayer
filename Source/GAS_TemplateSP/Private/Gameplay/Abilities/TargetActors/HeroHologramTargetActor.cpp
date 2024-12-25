// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/SC_HeroHologramController.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemGlobals.h"


void AHeroHologramTargetActor::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetInstigator());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	HeroHologramControllerComponent = HeroBase->GetHeroHologramControllerComponent();
	if (!HeroHologramControllerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("HologramAbilityHelperComponent is null in: %s"), *GetName());
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

void AHeroHologramTargetActor::OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnEnemyDetectionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == HeroBase)
	{
		return;
	}

	UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (!OtherActorASC)
	{
		return;
	}

	bIsTargetInRange = true;

	if (TargetLockSystemComponent->CurrentTarget)
	{
		if (TargetLockSystemComponent->CurrentTarget == OtherActor) 
		{
			// If the direction hasn't changed, we play the montage manually. This is for first time detection.
			if (!UpdateRelativeDirectionToTarget())
			{
				PlayMontageWithCallback(AttackMontage);
			}
		}
	}
	else
	{
		CurrentTarget = OtherActor;

		// If the direction hasn't changed, we play the montage manually. This is for first time detection.
		if (!UpdateRelativeDirectionToTarget())
		{
			PlayMontageWithCallback(AttackMontage);
		}
	}
	
}

void AHeroHologramTargetActor::OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEnemyDetectionEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor == HeroBase)
	{
		return;
	}

	UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (!OtherActorASC)
	{
		return;
	}

	if (SkeletalMesh->bPauseAnims)
	{
		SkeletalMesh->bPauseAnims = false;
	}

	if (TargetLockSystemComponent->CurrentTarget) 
	{
		if (OtherActor == TargetLockSystemComponent->CurrentTarget) 
		{
			if (AnimInstance)
			{
				AnimInstance->Montage_StopWithBlendOut(0.5f, AttackMontage);
			}
		}
	}
	else
	{
		CurrentTarget = nullptr;

		if (AnimInstance)
		{
			AnimInstance->Montage_StopWithBlendOut(0.5f, AttackMontage);
		}
	}

	bIsTargetInRange = false;
}




