// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/Shadows/HeroShadowTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"


void AHeroShadowTargetActor::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(InstigatorCharacter);
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	HeroShadowControllerComponent = HeroBase->GetHeroShadowControllerComponent();
	if (!HeroShadowControllerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShadowAbilityHelperComponent is null in: %s"), *GetName());
		return;
	}

	TargetLockSystemComponent = HeroBase->GetTargetLockSystemComponent();
	if (!TargetLockSystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLockSystemComponent is null in: %s"), *GetName());
		return;
	}
	TargetLockSystemComponent->OnTargetChanged.AddDynamic(this, &AHeroShadowTargetActor::OnTargetChaned);
	TargetLockSystemComponent->OnEndTargetLock.AddDynamic(this, &AHeroShadowTargetActor::OnEndTargetLock);

	AActor* CurrentLockedTarget = TargetLockSystemComponent->GetCurrentTarget();
	if (CurrentLockedTarget)
	{
		CurrentTarget = CurrentLockedTarget;
	}
}

void AHeroShadowTargetActor::Tick(float DeltaSeconds)
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

void AHeroShadowTargetActor::OnTargetChaned(AActor* NewTarget)
{
	BP_OnTargetChaned(NewTarget);
}

void AHeroShadowTargetActor::OnEndTargetLock()
{
	BP_OnEndTargetLock();
}

void AHeroShadowTargetActor::SyncRotationWithHero(float DeltaTime)
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

void AHeroShadowTargetActor::OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnEnemyDetectionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (!OtherActorASC)
	{
		return;
	}

	if (OtherActor == InstigatorCharacter)
	{
		return;
	}

	bIsTargetInRange = true;

	if (!TargetLockSystemComponent) 
	{
		return;
	}

	AActor* CurrentLockedTarget = TargetLockSystemComponent->GetCurrentTarget();
	if (CurrentLockedTarget)
	{
		if (CurrentLockedTarget == OtherActor)
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

void AHeroShadowTargetActor::OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEnemyDetectionEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (!OtherActorASC)
	{
		return;
	}

	if (OtherActor == InstigatorCharacter)
	{
		return;
	}

	if (SkeletalMesh->bPauseAnims)
	{
		SkeletalMesh->bPauseAnims = false;
	}

	if (!TargetLockSystemComponent) 
	{
		return;
	}

	if (TargetLockSystemComponent->GetCurrentTarget())
	{
		if (OtherActor == TargetLockSystemComponent->GetCurrentTarget())
		{
			if (AnimInstance)
			{
				AnimInstance->Montage_StopWithBlendOut(0.5f, AttackMontage);
			}
		}
	}
	else
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_StopWithBlendOut(0.5f, AttackMontage);
		}
	}

	CurrentTarget = nullptr;
	bIsTargetInRange = false;
}




