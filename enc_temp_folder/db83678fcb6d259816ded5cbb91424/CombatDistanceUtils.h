// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"

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

	// Binary kararlar için
	FORCEINLINE bool IsInRange(const AActor* A, const AActor* B, float Range)
	{
		if (Range <= 0.f)
		{
			return true;
		}

		const float Distance = GetDistance(A, B);
		return Distance <= Range;
	}
}
