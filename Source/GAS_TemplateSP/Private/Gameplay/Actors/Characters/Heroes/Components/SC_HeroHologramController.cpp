// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/SC_HeroHologramController.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Tags/GAS_Tags.h"

USC_HeroHologramController::USC_HeroHologramController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_HeroHologramController::BeginPlay()
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
	TargetLockSystem->OnStartTargetLock.AddDynamic(this, &USC_HeroHologramController::OnStartTargetLock);
	TargetLockSystem->OnEndTargetLock.AddDynamic(this, &USC_HeroHologramController::OnEndTargetLock);
}

void USC_HeroHologramController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HeroBase || !PC)
	{
		return;
	}
	
	CalculateCumulativeMouseInputs();
	UpdateTraceForwardDistance();

	bool IsHologramActive = HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram);
	if (!IsHologramActive || !HeroHologram)
	{
		return;
	}

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

FVector2D USC_HeroHologramController::CalculateCumulativeMouseInputs()
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

	CumulativeMouseDeltaY = FMath::Clamp(CumulativeMouseDeltaY, MinCumulativeMouseDeltaY, MaxCumulativeMouseDeltaY);
	return FVector2D(CumulativeMouseDeltaX, CumulativeMouseDeltaY);
}

void USC_HeroHologramController::LookAtTarget()
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetLockSystem->CurrentTarget->GetActorLocation());
	SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

void USC_HeroHologramController::SetHeroHologramLocation()
{
	if (bTargetLocked) 
	{
		FVector HeroHologramTargetLocation = GetHeroHologramLocationFromLineTraceTargetLocked();
		HeroHologramTargetLocation.Z += 90;
		HeroHologram->SetActorLocation(HeroHologramTargetLocation);
	}
	else
	{
		FVector HeroHologramTargetLocation = GetHeroHologramLocationFromLineTrace();
		HeroHologramTargetLocation.Z += 90;
		HeroHologram->SetActorLocation(HeroHologramTargetLocation);
	}
}

FVector USC_HeroHologramController::GetHeroHologramLocationFromLineTrace()
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

FVector USC_HeroHologramController::GetHeroHologramLocationFromLineTraceTargetLocked()
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

void USC_HeroHologramController::SetHologramLocationWithCumulativeMouseValuesTargetLocked()
{
	if (!HeroHologram || !TargetLockSystem->CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroHologram or CurrentTarget is null in: %s, Cannot set HologramLocation"), *GetName());
		return;
	}

	FVector OutClosestPoint;

	// Update new RightTraceDistance
	float PointDistToLine = GetPointDistToLine(HeroHologram, OutClosestPoint);
	ResetRightTraceDistance();
	SetCumulativeMouseDeltaXForRightTraceDistaneValue(PointDistToLine);

	// Update new ForwardTraceDistance
	float DistanceBetweenHologramAndHero = FVector::Dist(OutClosestPoint, HeroBase->GetActorLocation());
	ResetForwardTraceDistance();
	SetCumulativeMouseDeltaYForForwardTraceDistaneValue(DistanceBetweenHologramAndHero);
}

void USC_HeroHologramController::ResetForwardTraceDistance()
{
	CumulativeMouseDeltaY = 0;
	TraceForwardDistanceOffset = 0;
}

void USC_HeroHologramController::ResetRightTraceDistance()
{
	CumulativeMouseDeltaX = 0;
	TraceRightDistanceOffset = 0;
}

void USC_HeroHologramController::OnStartTargetLock()
{
	ResetRightTraceDistance();
	FVector OutClosestPoint;
	TraceRightDistanceOffset = GetPointDistToLine(HeroHologram, OutClosestPoint);
	TraceForwardDistanceOffset = CalculateTraceForwardDistanceOffset();
	bTargetLocked = true;
}

float USC_HeroHologramController::GetPointDistToLine(AActor* ReferanceActor, FVector& OutClosestPoint)
{
	if (!ReferanceActor || !TargetLockSystem->CurrentTarget)
	{
		return 0;
	}

	FVector HeroHologramLocation = HeroHologram->GetActorLocation();
	FVector TargetLocation = TargetLockSystem->CurrentTarget->GetActorLocation();
	FVector HeroLocation = HeroBase->GetActorLocation();

	// Adjust the target's Z coordinate to match the hero's Z coordinate, focusing only on the XY plane for direction calculation.
    // This effectively ignores the Z-axis difference, providing a direction vector confined to the horizontal plane.
	FVector HeroLocationUpdated = FVector(HeroLocation.X, HeroLocation.Y, TargetLocation.Z);
	FVector HeroHologramLocationUpdated = FVector(HeroHologramLocation.X, HeroHologramLocation.Y, TargetLocation.Z);

	FVector NormalizedDirection = HeroLocationUpdated - TargetLocation;
	NormalizedDirection.Normalize();

	float PointDistToLine = FMath::PointDistToLine(HeroHologramLocationUpdated, NormalizedDirection, HeroLocationUpdated, OutClosestPoint);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), HeroLocationUpdated, TargetLocation, FColor::Red, false, 2.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), OutClosestPoint, HeroHologramLocationUpdated, FColor::Blue, false, 2.0f, 0, 2.0f);
	}

	// Determines whether the hologram is to the right or left of a line defined by the normalized direction. 
    // Uses the cross product to check the Z-axis value: 
    // - If Z <= 0, the hologram is on the left side or on the line.
    // - If Z > 0, the hologram is on the right side.
    // Returns the distance to the line with a positive or negative sign based on the side.
	FVector PointDirection = HeroHologramLocation - HeroLocation;
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

float USC_HeroHologramController::CalculateTraceForwardDistanceOffset()
{
	float TraceForwardDistancePow = TraceForwardDistance * TraceForwardDistance;
	float TraceRightDistanceOffsetPow = TraceRightDistanceOffset * TraceRightDistanceOffset;
	return -(TraceForwardDistance - FMath::Sqrt(TraceForwardDistancePow - TraceRightDistanceOffsetPow));
}

void USC_HeroHologramController::OnEndTargetLock()
{
	bTargetLocked = false;
}

void USC_HeroHologramController::UpdateTraceForwardDistance()
{
	if (!C_TraceForwardDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_TraceForwardDistance->GetFloatValue(CumulativeMouseDeltaY);
	TraceForwardDistance = CurveValue + TraceForwardDistanceOffset;
}

void USC_HeroHologramController::UpdateTraceRightDistance()
{
	if (!C_TraceRightDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_TraceRightDistance->GetFloatValue(CumulativeMouseDeltaX);
	TraceRightDistance = CurveValue + TraceRightDistanceOffset;
}

void USC_HeroHologramController::SetCumulativeMouseDeltaXForRightTraceDistaneValue(float Value)
{
	// Getting value from reverse curve
	CumulativeMouseDeltaX = GetTimeForTraceRightDistance(Value);
}

void USC_HeroHologramController::SetCumulativeMouseDeltaYForForwardTraceDistaneValue(float Value)
{
	// Getting value from reverse curve
	CumulativeMouseDeltaY = GetTimeForTraceFowardDistance(Value);
}

float USC_HeroHologramController::GetTimeForTraceFowardDistance(float Value)
{
	if (!C_TraceForwardDistanceReverse)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceForwardDistanceReverse is null in: %s"), *GetName());
		return -1.f;
	}

	TArray<FRichCurveEditInfo> RichCurveEditInfo = C_TraceForwardDistanceReverse->GetCurves();
	FRealCurve* RealCurve = RichCurveEditInfo[0].CurveToEdit;

	float MinValue, MaxValue;
	RealCurve->GetValueRange(MinValue, MaxValue);

	float ClampedValue = FMath::Clamp(C_TraceForwardDistanceReverse->GetFloatValue(Value), MinValue, MaxValue);

	return ClampedValue;
}

float USC_HeroHologramController::GetTimeForTraceRightDistance(float Value)
{
	if (!C_TraceRightDistanceReverse)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceForwardDistanceReverse is null in: %s"), *GetName());
		return -1.f;
	}

	TArray<FRichCurveEditInfo> RichCurveEditInfo = C_TraceRightDistanceReverse->GetCurves();
	FRealCurve* RealCurve = RichCurveEditInfo[0].CurveToEdit;

	float MinValue, MaxValue;
	RealCurve->GetValueRange(MinValue, MaxValue);

	float ClampedValue = FMath::Clamp(C_TraceRightDistanceReverse->GetFloatValue(Value), MinValue, MaxValue);

	return ClampedValue;
}



