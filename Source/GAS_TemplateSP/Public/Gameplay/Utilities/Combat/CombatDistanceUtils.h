// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"

enum class EMovementRangeResult : uint8
{
	TooClose,
	InRange,
	TooFar
};

namespace CombatDistance
{
	// Ham mesafe (dereceli kararlar için)
	FORCEINLINE float GetDistance(const AActor* A, const AActor* B)
	{
		if (!IsValid(A) || !IsValid(B))
		{
			return TNumericLimits<float>::Max();
		}

		return FVector::Dist(A->GetActorLocation(), B->GetActorLocation());
	}

	FORCEINLINE float GetDistance2D(const AActor* A, const FVector& Location)
	{
		if (!IsValid(A))
		{
			return TNumericLimits<float>::Max();
		}

		return FVector::Dist2D(A->GetActorLocation(), Location);
	}

	// Binary kararlar için
	FORCEINLINE bool IsInRange(const AActor* A, const AActor* B, float Range)
	{
		if (!IsValid(A) || !IsValid(B) || Range < 0.f)
		{
			return false;
		}

		const float Distance = GetDistance(A, B);
		return Distance <= Range;
	}

	FORCEINLINE EMovementRangeResult EvaluateAttackRange(const AActor* Attacker, const AActor* Target, const float InMinRange, const float InMaxRange)
	{
		if (!IsValid(Attacker) || !IsValid(Target))
		{
			return EMovementRangeResult::TooFar;
		}

		const float Distance = CombatDistance::GetDistance(Attacker, Target);

		const float SafeMin = FMath::Max(0.f, InMinRange);
		const float SafeMax = FMath::Max(SafeMin, InMaxRange);

		if (Distance < SafeMin)
		{
			return EMovementRangeResult::TooClose;
		}

		if (Distance > SafeMax)
		{
			return EMovementRangeResult::TooFar;
		}

		return EMovementRangeResult::InRange;
	}
}
