// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/SC_HeroShadowController.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Abilities/TargetActors/Shadows/HeroShadowTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Tags/GAS_Tags.h"

USC_HeroShadowController::USC_HeroShadowController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_HeroShadowController::BeginPlay()
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
	TargetLockSystem->OnStartTargetLock.AddDynamic(this, &USC_HeroShadowController::OnStartTargetLock);
	TargetLockSystem->OnEndTargetLock.AddDynamic(this, &USC_HeroShadowController::OnEndTargetLock);
}

void USC_HeroShadowController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateCumulativeMouseInputs();
	OrientToPlayerView();
	UpdateTraceForwardDistance();
	
	if (!HeroBase)
	{
		return;
	}

	bool IsHologramActive = HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Shadow);
	if (!IsHologramActive || !HeroShadow)
	{
		return;
	}

	if (bTargetLocked)
	{
		LookAtTarget();
		UpdateTraceRightDistance();
	}

	SetHeroShadowLocation();
}

FVector2D USC_HeroShadowController::CalculateCumulativeMouseInputs()
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

void USC_HeroShadowController::OrientToPlayerView()
{
	if (PC) 
	{
		FVector PlayerViewLocation;
		FRotator PlayerViewRotation;
		PC->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
		SetWorldRotation(FRotator(0, PlayerViewRotation.Yaw, 0));
	}
}

void USC_HeroShadowController::LookAtTarget()
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetLockSystem->CurrentTarget->GetActorLocation());
	SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

void USC_HeroShadowController::SetHeroShadowLocation()
{
	if (!HeroShadow)
	{
		return;
	}

	FVector HeroShadowTargetLocation;
	if (bTargetLocked) 
	{
		HeroShadowTargetLocation = GetHeroShadowLocationFromLineTraceTargetLocked();
	}
	else
	{
		HeroShadowTargetLocation = GetHeroShadowLocationFromLineTrace();
	}

	HeroShadowTargetLocation.Z += 90;

	if(HeroShadow->IsValidLowLevel())
	HeroShadow->SetActorLocation(HeroShadowTargetLocation);
}

FVector USC_HeroShadowController::GetHeroShadowLocationFromLineTrace()
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

FVector USC_HeroShadowController::GetHeroShadowLocationFromLineTraceTargetLocked()
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

void USC_HeroShadowController::SetShadowLocationWithCumulativeMouseValuesTargetLocked()
{
	if (!HeroShadow || !TargetLockSystem->CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroHologram or CurrentTarget is null in: %s, Cannot set HologramLocation"), *GetName());
		return;
	}

	FVector OutClosestPoint;

	// Update new RightTraceDistance
	float PointDistToLine = GetPointDistToLine(HeroShadow, OutClosestPoint);
	ResetRightTraceDistance();
	SetCumulativeMouseDeltaXForRightTraceDistaneValue(PointDistToLine);

	// Update new ForwardTraceDistance
	float DistanceBetweenHologramAndHero = FVector::Dist(OutClosestPoint, HeroBase->GetActorLocation());
	ResetForwardTraceDistance();
	SetCumulativeMouseDeltaYForForwardTraceDistaneValue(DistanceBetweenHologramAndHero);
}

void USC_HeroShadowController::ResetForwardTraceDistance()
{
	CumulativeMouseDeltaY = 0;
	TraceForwardDistanceOffset = 0;
}

void USC_HeroShadowController::ResetRightTraceDistance()
{
	CumulativeMouseDeltaX = 0;
	TraceRightDistanceOffset = 0;
}

void USC_HeroShadowController::OnStartTargetLock()
{
	ResetRightTraceDistance();
	FVector OutClosestPoint;
	TraceRightDistanceOffset = GetPointDistToLine(HeroShadow, OutClosestPoint);
	TraceForwardDistanceOffset = CalculateTraceForwardDistanceOffset();
	bTargetLocked = true;
}

float USC_HeroShadowController::GetPointDistToLine(AActor* ReferanceActor, FVector& OutClosestPoint)
{
	if (!ReferanceActor || !TargetLockSystem->CurrentTarget || !HeroShadow->IsValidLowLevel())
	{
		return 0;
	}
	
	FVector HeroShadowLocation = HeroShadow.Get()->GetActorLocation();
	FVector TargetLocation = TargetLockSystem->CurrentTarget->GetActorLocation();
	FVector HeroLocation = HeroBase->GetActorLocation();

	// Adjust the target's Z coordinate to match the hero's Z coordinate, focusing only on the XY plane for direction calculation.
    // This effectively ignores the Z-axis difference, providing a direction vector confined to the horizontal plane.
	FVector HeroLocationUpdated = FVector(HeroLocation.X, HeroLocation.Y, TargetLocation.Z);
	FVector HeroShadowLocationUpdated = FVector(HeroShadowLocation.X, HeroShadowLocation.Y, TargetLocation.Z);

	FVector NormalizedDirection = HeroLocationUpdated - TargetLocation;
	NormalizedDirection.Normalize();

	float PointDistToLine = FMath::PointDistToLine(HeroShadowLocationUpdated, NormalizedDirection, HeroLocationUpdated, OutClosestPoint);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), HeroLocationUpdated, TargetLocation, FColor::Red, false, 2.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), OutClosestPoint, HeroShadowLocationUpdated, FColor::Blue, false, 2.0f, 0, 2.0f);
	}

	// Determines whether the hologram is to the right or left of a line defined by the normalized direction. 
    // Uses the cross product to check the Z-axis value: 
    // - If Z <= 0, the hologram is on the left side or on the line.
    // - If Z > 0, the hologram is on the right side.
    // Returns the distance to the line with a positive or negative sign based on the side.
	FVector PointDirection = HeroShadowLocation - HeroLocation;
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

float USC_HeroShadowController::CalculateTraceForwardDistanceOffset()
{
	float TraceForwardDistancePow = TraceForwardDistance * TraceForwardDistance;
	float TraceRightDistanceOffsetPow = TraceRightDistanceOffset * TraceRightDistanceOffset;
	return -(TraceForwardDistance - FMath::Sqrt(TraceForwardDistancePow - TraceRightDistanceOffsetPow));
}

void USC_HeroShadowController::OnEndTargetLock()
{
	bTargetLocked = false;
}

void USC_HeroShadowController::UpdateTraceForwardDistance()
{
	if (!C_TraceForwardDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_TraceForwardDistance->GetFloatValue(CumulativeMouseDeltaY);
	TraceForwardDistance = CurveValue + TraceForwardDistanceOffset;
}

void USC_HeroShadowController::UpdateTraceRightDistance()
{
	if (!C_TraceRightDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_TraceRightDistance->GetFloatValue(CumulativeMouseDeltaX);
	TraceRightDistance = CurveValue + TraceRightDistanceOffset;
}

void USC_HeroShadowController::SetCumulativeMouseDeltaXForRightTraceDistaneValue(float Value)
{
	// Getting value from reverse curve
	CumulativeMouseDeltaX = GetTimeForTraceRightDistance(Value);
}

void USC_HeroShadowController::SetCumulativeMouseDeltaYForForwardTraceDistaneValue(float Value)
{
	// Getting value from reverse curve
	CumulativeMouseDeltaY = GetTimeForTraceFowardDistance(Value);
}

float USC_HeroShadowController::GetTimeForTraceFowardDistance(float Value)
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

float USC_HeroShadowController::GetTimeForTraceRightDistance(float Value)
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



