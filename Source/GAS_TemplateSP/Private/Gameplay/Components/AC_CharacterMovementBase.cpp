// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_CharacterMovementBase.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include <AbilitySystemGlobals.h>
#include "DrawDebugHelpers.h"


float UAC_CharacterMovementBase::SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact)
{
    return Super::SlideAlongSurface(Delta, Time, Normal, Hit, bHandleImpact);

    /*
    if (const AActor* OtherActor = Hit.GetActor())
    {
        if (!OtherActor->IsA<ACharacter>())
        {
            return Super::SlideAlongSurface(Delta, Time, Normal, Hit, bHandleImpact);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("🟡 SlideAlongSurface - Character Hit"));

    // Normal'in dikey bileşenini kontrol et
    float NormalZ = Hit.Normal.Z;

    // Kayma faktörü: ne kadar yataysa o kadar hızlı kayar
    // NormalZ: -1.0 = taban (hızlı), 0.0 = yan duvar (orta), 1.0 = tavan (yavaş)
    float SlideFactor = FMath::Clamp(1.0f - FMath::Abs(NormalZ), 0.3f, 1.0f);

    // Slide direction hesapla
    FVector SlideDirection = FVector::VectorPlaneProject(Delta, Hit.Normal);
    SlideDirection.Normalize();

    // Gravity assist ekle (hep aşağı çeker)
    float GravityMagnitude = FMath::Abs(GetGravityZ());
    SlideDirection.Z -= GravityMagnitude * Time * SlideFactor * 0.5f;

    // Yeni velocity hesapla
    float CurrentSpeed = Velocity.Size();
    FVector NewVelocity = SlideDirection * CurrentSpeed * SlideFactor;

    if (NewVelocity.Size() < MinSlideSpeed)
    {
        NewVelocity = SlideDirection * MinSlideSpeed;
    }

    Velocity = NewVelocity;

    UE_LOG(LogTemp, Warning, TEXT("   Slide Factor: %.2f, New Vel: %s"),
        SlideFactor, *Velocity.ToString());

    // Super'i çağır ama bHandleImpact = false
    return Super::SlideAlongSurface(Delta, Time, Hit.Normal, Hit, false);
    */
}

FVector UAC_CharacterMovementBase::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
    if (const AActor* OtherActor = Hit.GetActor())
    {
        if (!OtherActor->IsA<ACharacter>())
        {
            return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
        }
    }

    // Kapsülün merkezini ve çarpma noktasını al
    FVector CapsuleCenter = Hit.GetActor()->GetActorLocation();
    FVector ImpactPoint = Hit.ImpactPoint;

    // Çarpma noktasından kapsül merkezine olan radyal yönü hesapla
    FVector RadialDirection = (ImpactPoint - CapsuleCenter).GetSafeNormal();

    // Yükseklik faktörünü hesapla
    float HitHeight = ImpactPoint.Z - CapsuleCenter.Z;
    float CapsuleHalfHeight = 88.f;
    float NormalizedHeight = FMath::Clamp(HitHeight / CapsuleHalfHeight, 0.f, 1.f);

    // Teğetsel kayma yönünü hesapla (yüzey boyunca kayma)
    // Normal'e dik olan ve aşağı yöne sahip vektör
    FVector Down = FVector(0, 0, -1.f);
    FVector TangentSlide = (Down - Normal * FVector::DotProduct(Down, Normal)).GetSafeNormal();

    // Radyal yön ile teğetsel kayma yönünü birleştir
    // Yukarıda daha fazla radyal, aşağıda daha fazla teğetsel
    float RadialWeight = NormalizedHeight * 0.3f; // Kubbenin üstünde biraz dışa doğru it
    FVector SlideDir = FMath::Lerp(TangentSlide, RadialDirection, RadialWeight).GetSafeNormal();

    // Orijinal hızı koru ve kayma yönüne uygula
    float DeltaMagnitude = Delta.Size();
    FVector SlideVec = SlideDir * DeltaMagnitude;

    // Yerçekimi etkisini ekle
    float GravityFactor = FMath::Pow(1.f - NormalizedHeight, 2.f); // Aşağıda daha fazla yerçekimi
    SlideVec.Z += GetGravityZ() * Time * GravityFactor;

    // Debug çizimi
    FVector Start = ImpactPoint;
    FVector End = Start + SlideVec * 50.f;
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0, 2.f);
    DrawDebugPoint(GetWorld(), Start, 12.f, FColor::Yellow, false, 2.f);
    DrawDebugLine(GetWorld(), Start, Start + Normal * 50.f, FColor::Blue, false, 2.f, 0, 1.f); // Normal
    DrawDebugLine(GetWorld(), Start, Start + RadialDirection * 50.f, FColor::Green, false, 2.f, 0, 1.f); // Radyal

    return SlideVec;
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
