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

	bool IsHologramActive = HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram);
	if (!IsHologramActive) 
	{
		return;
	}

	if (!HeroHologramTargetActor)
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

void USC_HeroHologramController::SetHeroHologramLocation()
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

void USC_HeroHologramController::ResetRightTraceDistance()
{
	CumulativeMouseDeltaX = 0;
	TraceRightDistanceOffset = 0;
}

void USC_HeroHologramController::OnStartTargetLock()
{
	CumulativeMouseDeltaX = 0;
	TraceRightDistance = 0;
	TraceRightDistanceOffset = GetPointDistToLine();
	TraceForwardDistanceOffset = CalculateTraceForwardDistanceOffset();
	bTargetLocked = true;
}

float USC_HeroHologramController::GetPointDistToLine()
{
	if (!HeroHologramTargetActor)
	{
		return 0;
	}

	FVector HeroLocation = HeroBase->GetActorLocation();
	FVector TargetLocation = TargetLockSystem->CurrentTarget->GetActorLocation();

	// Adjust the target's Z coordinate to match the hero's Z coordinate, focusing only on the XY plane for direction calculation.
    // This effectively ignores the Z-axis difference, providing a direction vector confined to the horizontal plane.
	FVector NormalizedDirection = HeroLocation - FVector(TargetLocation.X, TargetLocation.Y, HeroLocation.Z);
	NormalizedDirection.Normalize();

	FVector ClosestPointOnLine;
	float PointDistToLine = FMath::PointDistToLine(HeroHologramTargetActor->GetActorLocation(), NormalizedDirection, HeroBase->GetActorLocation(), ClosestPointOnLine);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), HeroLocation, FVector(TargetLocation.X, TargetLocation.Y, HeroLocation.Z), FColor::Red, false, 2.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), ClosestPointOnLine, HeroHologramTargetActor->GetActorLocation(), FColor::Red, false, 2.0f, 0, 2.0f);
	}

	// Determines whether the hologram is to the right or left of a line defined by the normalized direction. 
    // Uses the cross product to check the Z-axis value: 
    // - If Z <= 0, the hologram is on the left side or on the line.
    // - If Z > 0, the hologram is on the right side.
    // Returns the distance to the line with a positive or negative sign based on the side.
	FVector PointDirection = HeroHologramTargetActor->GetActorLocation() - HeroLocation;
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
	TraceRightDistance = 0;
	TraceForwardDistanceOffset = 0;
	bTargetLocked = false;
}

void USC_HeroHologramController::UpdateTraceForwardDistance()
{
	if (!C_MouseInoutSensitiveY)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_MouseInoutSensitiveY->GetFloatValue(CumulativeMouseDeltaY);
	TraceForwardDistance = CurveValue + TraceForwardDistanceOffset;
}

void USC_HeroHologramController::UpdateTraceRightDistance()
{
	if (!C_MouseInoutSensitiveX)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return;
	}

	float CurveValue = C_MouseInoutSensitiveX->GetFloatValue(CumulativeMouseDeltaX);
	TraceRightDistance = CurveValue + TraceRightDistanceOffset;
}

void USC_HeroHologramController::LookAtTarget()
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetLockSystem->CurrentTarget->GetActorLocation());
	SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

