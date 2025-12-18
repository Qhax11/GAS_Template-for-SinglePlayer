// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"


namespace CombatDistance
{
    FORCEINLINE bool IsInRange(const AActor* Source, const AActor* Target, float Range)
    {
        if (!IsValid(Source) || !IsValid(Target))
        {
            return false;
        }

        return FVector::DistSquared(Source->GetActorLocation(), Target->GetActorLocation()) <= FMath::Square(Range);
    }
}
