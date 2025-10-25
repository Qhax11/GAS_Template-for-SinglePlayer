// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_CharacterMovementBase.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include <AbilitySystemGlobals.h>
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"


void UAC_CharacterMovementBase::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
    Super::HandleImpact(Hit, TimeSlice, MoveDelta);

    /*
    if (ACharacter* Other = Cast<ACharacter>(Hit.GetActor()))
    {
        // Çarpışma yönüne ters yöne kayma ekle
        const FVector SlideDir = FVector::VectorPlaneProject(MoveDelta, Hit.Normal).GetSafeNormal();
        const float SlideForce = 200.f;

         Velocity += SlideDir * SlideForce;
    }
    */
}

float UAC_CharacterMovementBase::SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact)
{

    return 0;
}

FVector UAC_CharacterMovementBase::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
     if (const AActor* OtherActor = Hit.GetActor())
    {
        if (OtherActor->IsA<ACharacter>())
        {
            FVector DefaultResult = Super::ComputeSlideVector(Delta, Time, Normal, Hit);
            DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + DefaultResult * 10.1f, FColor::Red, false, 2.f, 0, 2.f);
            DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 12.f, FColor::Yellow, false, 2.f);
            return DefaultResult;
        }
    }

    return Super::ComputeSlideVector(Delta, Time, Normal, Hit);

    /*
    if (const AActor* OtherActor = Hit.GetActor())
    {
        if (!OtherActor->IsA<ACharacter>())
        {
            return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
        }
    }

    FVector SlideDir = FVector(0, 0, -1.f); // sabit aşağı yön

    // Eğer biraz yana doğru bir eğim istiyorsan, sabit bir offset ekleyebilirsin:
    SlideDir = (SlideDir + FVector(0.2f, 0.f, 0.f)).GetSafeNormal(); // X yönünde 0.2 eğim

    FVector Result = SlideDir * SlideSpeedMultiplier * Time; // zamanla ölçeklendir

    // Debug
    DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Result * 0.1f, FColor::Red, false, 2.f, 0, 2.f);
    DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 12.f, FColor::Yellow, false, 2.f);

    return Result;
    */
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

void UAC_CharacterMovementBase::DebugLogMovement(const FString& FunctionName, const FVector& VelocityBefore, const FVector& VelocityAfter, const FHitResult* Hit)
{
    FVector VelChange = VelocityAfter - VelocityBefore;

    if (VelChange.Size() > 10.f) // Önemli değişiklik varsa
    {
        UE_LOG(LogTemp, Warning, TEXT("=== %s ==="), *FunctionName);
        UE_LOG(LogTemp, Warning, TEXT("Velocity Before: %s"), *VelocityBefore.ToString());
        UE_LOG(LogTemp, Warning, TEXT("Velocity After: %s"), *VelocityAfter.ToString());
        UE_LOG(LogTemp, Warning, TEXT("Change: %s (Size: %.2f)"),
            *VelChange.ToString(), VelChange.Size());

        if (Hit)
        {
            UE_LOG(LogTemp, Warning, TEXT("Hit Normal: %s"), *Hit->Normal.ToString());
            UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *Hit->ImpactPoint.ToString());
        }
    }
}
