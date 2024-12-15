// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/SC_HologramAbilityHelper.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Tags/GAS_Tags.h"

USC_HologramAbilityHelper::USC_HologramAbilityHelper()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_HologramAbilityHelper::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PC is null in: %s"), *GetName());
		return;
	}

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	TargetLockSystem = HeroBase->GetTargetLockSystemComponent();
	if (!TargetLockSystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLockSystem is null in: %s"), *GetName());
		return;
	}
	TargetLockSystem->OnStartTargetLock.AddDynamic(this, &USC_HologramAbilityHelper::OnStartTargetLock);
	TargetLockSystem->OnEndTargetLock.AddDynamic(this, &USC_HologramAbilityHelper::OnEndTargetLock);
}

void USC_HologramAbilityHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HeroBase || !PC)
	{
		return;
	}
	
	CalculateCumulativeMouseInputs();

	if (!HeroHologramTargetActor->IsValidLowLevel())
	{
		return;
	}

	UpdateTraceForwardDistance();

	if (bTargetLocked)
	{
		LookAtTarget();
		UpdateTraceRightDistance();
	}
	else 
	{
		FVector PlayerViewLocation;
		FRotator PlayerViewRotation;
		PC->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
		SetWorldRotation(FRotator(0, PlayerViewRotation.Yaw, 0));
	}

	SetHeroHologramLocation();
}

void USC_HologramAbilityHelper::SetHeroHologramLocation()
{
	if (bTargetLocked) 
	{
		FVector HeroHologramTargetLocation = GetHeroHologramLocationFromLineTraceTargetLocked();
		HeroHologramTargetLocation.Z += 90;
		HeroHologramTargetActor->SetActorLocation(HeroHologramTargetLocation);
	}
	else
	{
		FVector HeroHologramTargetLocation = GetHeroHologramLocationFromLineTrace();
		HeroHologramTargetLocation.Z += 90;
		HeroHologramTargetActor->SetActorLocation(HeroHologramTargetLocation);
	}
}

FVector USC_HologramAbilityHelper::GetHeroHologramLocationFromLineTrace()
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	FVector FirstTraceStart = GetComponentLocation();
	FVector FirstTraceEnd = FirstTraceStart + GetForwardVector() * TraceForwardDistance;

	GetWorld()->LineTraceSingleByChannel(HitResult, FirstTraceStart, FirstTraceEnd, ECC_Visibility, CollisionParams);
	if (bDrawDebug) 
	{
		DrawDebugLine(GetWorld(), FirstTraceStart, FirstTraceEnd, FColor::Blue, false, 0.0f, 0, 1.0f);
	}

	FVector SecondTraceStart = HitResult.TraceEnd;
	FVector SecondTraceEnd = HitResult.TraceEnd;
	SecondTraceEnd.Z *= -1.0f;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, SecondTraceStart, SecondTraceEnd, ECC_Visibility, CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), SecondTraceStart, SecondTraceEnd, FColor::Blue, false, 0.0f, 0, 1.0f);
		if (bHit)
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Blue, false, 0.0f);
		}
	}

	return HitResult.ImpactPoint;
}

FVector USC_HologramAbilityHelper::GetHeroHologramLocationFromLineTraceTargetLocked()
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	FVector FirstTraceStart = GetComponentLocation();
	FVector FirstTraceEnd = FirstTraceStart + GetRightVector() * TraceRightDistance;

	GetWorld()->LineTraceSingleByChannel(HitResult, FirstTraceStart, FirstTraceEnd, ECC_Visibility, CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), FirstTraceStart, FirstTraceEnd, FColor::Red, false, 0.0f, 0, 1.0f);
	}

	FVector SecondTraceStart = HitResult.TraceEnd;
	FVector SecondTraceEnd = SecondTraceStart + GetForwardVector() * TraceForwardDistance;

	GetWorld()->LineTraceSingleByChannel(HitResult, SecondTraceStart, SecondTraceEnd, ECC_Visibility, CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), SecondTraceStart, SecondTraceEnd, FColor::Red, false, 0.0f, 0, 1.0f);
	}

	FVector ThirdTraceStart = HitResult.TraceEnd;
	FVector ThirdTraceEnd = HitResult.TraceEnd;
	ThirdTraceEnd.Z *= -1.0f;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ThirdTraceStart, ThirdTraceEnd, ECC_Visibility, CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), ThirdTraceStart, ThirdTraceEnd, FColor::Red, false, 0.0f, 0, 1.0f);
		if (bHit)
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 0.0f);
		}
	}

	return HitResult.ImpactPoint;
}

FVector2D USC_HologramAbilityHelper::CalculateCumulativeMouseInputs()
{
	if (!PC)
	{
		return FVector2D();
	}

	FVector2D MouseInput;
	PC->GetInputMouseDelta(MouseInput.X, MouseInput.Y);
	if (!MouseInput.IsNearlyZero())
	{
		CumulativeMouseDeltaX += MouseInput.X * SensitiveMultiplierX;
		CumulativeMouseDeltaY += MouseInput.Y * SensitiveMultiplierY;
	}

	return FVector2D(CumulativeMouseDeltaX, CumulativeMouseDeltaY);
}

void USC_HologramAbilityHelper::OnStartTargetLock()
{
	CumulativeMouseDeltaX = 0;
	TraceRightDistance = 0;
	TraceRightDistanceOffset = GetPointDistToLine();
	TraceForwardDistanceOffset = CalculateTraceForwardDistanceOffset();
	bTargetLocked = true;
}

float USC_HologramAbilityHelper::GetPointDistToLine()
{
	if (!HeroHologramTargetActor)
	{
		return 0;
	}
	FVector ClosestPointOnLine;
	FVector NormalizedDirection = HeroBase->GetActorLocation() - TargetLockSystem->CurrentTarget->GetActorLocation();
	NormalizedDirection.Normalize();
	float PointDistToLine = FMath::PointDistToLine(HeroHologramTargetActor->GetActorLocation(), NormalizedDirection, HeroBase->GetActorLocation(), ClosestPointOnLine);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), HeroBase->GetActorLocation(), TargetLockSystem->CurrentTarget->GetActorLocation(), FColor::Red, false, 2.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), ClosestPointOnLine, HeroHologramTargetActor->GetActorLocation(), FColor::Red, false, 2.0f, 0, 2.0f);
	}

	FVector PointDirection = HeroHologramTargetActor->GetActorLocation() - HeroBase->GetActorLocation();
	FVector CrossProductResult = FVector::CrossProduct(NormalizedDirection, PointDirection);

	if (CrossProductResult.Z <= 0)
	{
		return PointDistToLine;
	}
	else
	{
		return -PointDistToLine;
	}
}

float USC_HologramAbilityHelper::CalculateTraceForwardDistanceOffset()
{
	float TraceForwardDistancePow = TraceForwardDistance * TraceForwardDistance;
	float TraceRightDistanceOffsetPow = TraceRightDistanceOffset * TraceRightDistanceOffset;
	return -(TraceForwardDistance - FMath::Sqrt(TraceForwardDistancePow - TraceRightDistanceOffsetPow));
}

void USC_HologramAbilityHelper::OnEndTargetLock()
{
	TraceRightDistance = 0;
	TraceForwardDistanceOffset = 0;
	bTargetLocked = false;
}

void USC_HologramAbilityHelper::UpdateTraceForwardDistance()
{
	if (!C_MouseInoutSensitiveY)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_MouseInoutSensitiveY->GetFloatValue(CumulativeMouseDeltaY);
	TraceForwardDistance = CurveValue + TraceForwardDistanceOffset;
}

void USC_HologramAbilityHelper::UpdateTraceRightDistance()
{
	if (!C_MouseInoutSensitiveX)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_MouseInoutSensitiveX->GetFloatValue(CumulativeMouseDeltaX);
	TraceRightDistance = CurveValue + TraceRightDistanceOffset;
}

void USC_HologramAbilityHelper::LookAtTarget()
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetLockSystem->CurrentTarget->GetActorLocation());
	SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

