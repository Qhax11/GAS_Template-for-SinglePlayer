// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_CharacterMovementBase.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include <AbilitySystemGlobals.h>
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

FVector UAC_CharacterMovementBase::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
     if (const AActor* OtherActor = Hit.GetActor())
    {
        if (OtherActor->IsA<ACharacter>())
        {
            FVector DefaultResult = Super::ComputeSlideVector(Delta, Time, Normal, Hit);
            if (bEnableDebug) 
            {
                DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + DefaultResult * 10.1f, FColor::Red, false, 2.f, 0, 2.f);
                DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 12.f, FColor::Yellow, false, 2.f);
            }
            return DefaultResult;
        }
    }

    return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
}

bool UAC_CharacterMovementBase::ShouldCheckForValidLandingSpot(float DeltaTime, const FVector& Delta, const FHitResult& Hit) const
{
    UE_LOG(LogTemp, Error, TEXT("🔴 ShouldCheckForValidLandingSpot - Hit Actor: %s"),
        Hit.GetActor() ? *Hit.GetActor()->GetName() : TEXT("None"));

    if (Hit.GetActor() && Hit.GetActor()->IsA<ACharacter>())
    {
        UE_LOG(LogTemp, Error, TEXT("🔴 BLOCKING Landing on Character!"));
        return false; // Karakter üstünde landing kontrol etme!
    }

    return Super::ShouldCheckForValidLandingSpot(DeltaTime, Delta, Hit);
}

void UAC_CharacterMovementBase::FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult) const
{
    Super::FindFloor(CapsuleLocation, OutFloorResult, bCanUseCachedLocation,
        DownwardSweepResult);

    // Eğer floor enemy ise, invalid yap
    if (OutFloorResult.bBlockingHit && OutFloorResult.HitResult.GetActor())
    {
        if (OutFloorResult.HitResult.GetActor()->IsA<ACharacter>())
        {
            UE_LOG(LogTemp, Warning, TEXT("🟡 FindFloor - Invalidating Character Floor!"));

            // Floor'u tamamen invalid yap
            OutFloorResult.Clear();
            OutFloorResult.bBlockingHit = false;
            OutFloorResult.bWalkableFloor = false;
            OutFloorResult.bLineTrace = false;
            OutFloorResult.FloorDist = BIG_NUMBER;
            OutFloorResult.LineDist = BIG_NUMBER;
        }
    }
}





