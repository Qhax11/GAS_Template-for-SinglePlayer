// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroAimAssist.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Utilities/GAS_UtilityLibrary.h"


UAC_HeroAimAssist::UAC_HeroAimAssist()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_HeroAimAssist::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	if (!HeroBase || !HeroASC || !HeroTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase, HeroASC or HeroTagDelegatesComp is null in: %s)"), *GetName());
		return;
	}

	HeroController = HeroBase->GetHeroControlComponent();
	if (!HeroController) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroController is null in: %s)"), *GetName());
		return;
	}

	HeroTargetLock = HeroBase->GetTargetLockSystemComponent();
	if (!HeroTargetLock)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroTargetLock is null in: %s)"), *GetName());
		return;
	}

	if (!TraceCheckEnemy) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceCheckEnemy is null in: %s)"), *GetName());
		return;
	}

	HeroASC->AbilityActivatedCallbacks.AddUObject(this, &UAC_HeroAimAssist::OnHeroAbilityActivated);
}

void UAC_HeroAimAssist::OnHeroAbilityActivated(UGameplayAbility* Ability) 
{
	if (!Ability)
	{
		return;
	}

	// Sadece melee attack ability'lerde çalýþ
	UGA_MeleeAttackBase* MeleeAttackAbility = Cast<UGA_MeleeAttackBase>(Ability);
	if (!MeleeAttackAbility)
	{
		return;
	}

	bool HeroTargetLocked = HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked);
	bool HeroOrientRotationToMovement = HeroController->bOrientRotationToMovement;

	// DURUM 1: Target Lock VARSA ve Orient Rotation AKTÝFSE
	if (HeroTargetLocked && HeroOrientRotationToMovement)
	{
		return; // Hiçbir þey yapma, zaten otomatik dönüyor
	}

	// DURUM 2: Target Lock VARSA ama Orient Rotation KAPALI
	if (HeroTargetLocked && !HeroOrientRotationToMovement)
	{
		// Locked target'a dön
		if (HeroTargetLock && HeroTargetLock->CurrentTarget)
		{
			TargetActor = HeroTargetLock->CurrentTarget;
			bIsRotating = true;
			SetComponentTickEnabled(true);
		}
		return;
	}

	// DURUM 3: Target Lock YOKSA - En yakýn düþmaný bul ve dön
	TArray<AActor*> OutResultActors;
	TraceCheckEnemy->Trace->CreateTraceWithTeamFilter(GetWorld(), HeroBase, ETeamAttitude::Hostile, OutResultActors);

	if (OutResultActors.IsEmpty())
	{
		return;
	}

	UGAS_UtilityLibrary::FilterOutDeadActors(OutResultActors);
	TargetActor = UGAS_UtilityLibrary::FindNearestActor(HeroBase, OutResultActors);

	if (!TargetActor)
	{
		return;
	}

	// Açý kontrolü yap
	if (IsTargetInAngle())
	{
		bIsRotating = true;
		SetComponentTickEnabled(true);
	}
}

bool UAC_HeroAimAssist::IsTargetInAngle()
{
	if (!HeroBase || !TargetActor)
	{
		return false;
	}

	FVector HeroForward = HeroBase->GetActorForwardVector();

	FVector ToTarget = (TargetActor->GetActorLocation() - HeroBase->GetActorLocation()).GetSafeNormal();

	float AngleDifference = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(HeroForward, ToTarget)));

	return AngleDifference <= AssistMaxAngle;
}

void UAC_HeroAimAssist::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsRotating || !TargetActor || !HeroBase)
	{
		StopRotation();
		return;
	}

	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector HeroLocation = HeroBase->GetActorLocation();

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(HeroLocation, TargetLocation);
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;

	FRotator CurrentRotation = HeroBase->GetActorRotation();

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, AssistRotationSpeed);

	HeroBase->SetActorRotation(NewRotation);

	if (FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw, 2.0f))
	{
		StopRotation();
	}
}

void UAC_HeroAimAssist::StopRotation()
{
	bIsRotating = false;
	TargetActor = nullptr;
	SetComponentTickEnabled(false);
}