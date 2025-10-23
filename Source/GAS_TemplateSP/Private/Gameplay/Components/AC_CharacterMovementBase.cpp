// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_CharacterMovementBase.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include <AbilitySystemGlobals.h>




void UAC_CharacterMovementBase::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
    if (const AActor* OtherActor = Hit.GetActor())
    {
        if (OtherActor->IsA<ACharacter>())
        {
            UE_LOG(LogTemp, Error, TEXT("🔴 HandleImpact CALLED"));

            // Impact'i tamamen ignore et - BİTTİ!
            return;
        }
    }

    Super::HandleImpact(Hit, TimeSlice, MoveDelta);
}

bool UAC_CharacterMovementBase::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation)
{
    FVector VelBefore = Velocity;

    if (const AActor* OtherActor = Hit.GetActor())
    {
        if (OtherActor->IsA<ACharacter>())
        {
            UE_LOG(LogTemp, Error, TEXT("🔴 ResolvePenetration CALLED - Adjustment: %s"),
                *Adjustment.ToString());

            FVector MinAdjustment = Adjustment;
            MinAdjustment.Z = FMath::Min(MinAdjustment.Z, 0.f);
            MinAdjustment *= 0.1f;

            bool Result = Super::ResolvePenetrationImpl(MinAdjustment, Hit, NewRotation);

            DebugLogMovement(TEXT("ResolvePenetration"), VelBefore, Velocity, &Hit);
            return Result;
        }
    }

    return Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotation);
}

float UAC_CharacterMovementBase::SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact)
{
    if (const AActor* OtherActor = Hit.GetActor())
    {
        if (OtherActor->IsA<ACharacter>())
        {
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

            // Minimum kayma hızı (çok yavaş kalmasın)
            float MinSlideSpeed = 300.0f;
            if (NewVelocity.Size() < MinSlideSpeed)
            {
                NewVelocity = SlideDirection * MinSlideSpeed;
            }

            Velocity = NewVelocity;

            UE_LOG(LogTemp, Warning, TEXT("   Slide Factor: %.2f, New Vel: %s"),
                SlideFactor, *Velocity.ToString());

            // Super'i çağır ama bHandleImpact = false
            return Super::SlideAlongSurface(Delta, Time, Hit.Normal, Hit, false);
        }
    }

    return Super::SlideAlongSurface(Delta, Time, Normal, Hit, bHandleImpact);
}

FVector UAC_CharacterMovementBase::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
    if (const AActor* OtherActor = Hit.GetActor())
    {
        if (OtherActor->IsA<ACharacter>())
        {
            // Capsule oval yüzeyini rampa gibi hesapla

            // Hit noktasının yüksekliğine göre eğim belirle
            float HitHeight = Hit.ImpactPoint.Z - Hit.GetActor()->GetActorLocation().Z;
            float CapsuleHalfHeight = 88.f; // Character capsule half height (ayarla!)

            // Normalize height (0 = en alt, 1 = en üst)
            float NormalizedHeight = FMath::Clamp(HitHeight / CapsuleHalfHeight, 0.f, 1.f);

            // Yükseklik ne kadar fazlaysa eğim o kadar az (daha yatay)
            // 0 = dik duvar, 1 = düz zemin
            float SlopeFactor = FMath::Pow(NormalizedHeight, 2.f); // Quadratic curve

            // Modified normal oluştur
            FVector ModifiedNormal = Normal;
            ModifiedNormal.Z = FMath::Lerp(Normal.Z, -1.f, SlopeFactor); // Üste doğru daha yatay
            ModifiedNormal.Normalize();

            // Slide vektörü hesapla
            FVector SlideVec = FVector::VectorPlaneProject(Delta, ModifiedNormal);

            // Gravity ekle (kayma efekti)
            SlideVec.Z += GetGravityZ() * Time * SlopeFactor;

            return SlideVec;
        }
    }

    return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
}

void UAC_CharacterMovementBase::PhysWalking(float deltaTime, int32 Iterations)
{
    FVector VelBefore = Velocity;
    Super::PhysWalking(deltaTime, Iterations);

    // Karakter collision sonrası velocity değişimini kontrol et
    if (CurrentFloor.HitResult.GetActor() &&
        CurrentFloor.HitResult.GetActor()->IsA<ACharacter>())
    {
        UE_LOG(LogTemp, Error, TEXT("🔴 PhysWalking - Standing on Character!"));
        DebugLogMovement(TEXT("PhysWalking"), VelBefore, Velocity);

        // Floor'u invalid yap
        CurrentFloor.Clear();
        SetMovementMode(MOVE_Falling);
    }
}

void UAC_CharacterMovementBase::PhysFalling(float deltaTime, int32 Iterations)
{
    FVector VelBefore = Velocity;

    Super::PhysFalling(deltaTime, Iterations);

    // LANDING KONTROLÜ
    if (MovementMode != MOVE_Falling) // Landing oldu!
    {
        if (CurrentFloor.HitResult.GetActor() &&
            CurrentFloor.HitResult.GetActor()->IsA<ACharacter>())
        {
            UE_LOG(LogTemp, Error, TEXT("🔴🔴🔴 CAUGHT LANDING ON CHARACTER - FORCE FALLING!"));

            // Floor'u temizle
            CurrentFloor.Clear();

            // Zorla falling mode
            SetMovementMode(MOVE_Falling);

            // Velocity'yi geri yükle ve kayma ekle
            Velocity = VelBefore;
            Velocity.Z = FMath::Min(VelBefore.Z, -200.f); // Minimum aşağı hız

            // Hit normal'ine göre kayma yönü ekle
            if (CurrentFloor.HitResult.bBlockingHit)
            {
                FVector SlideDir = FVector::VectorPlaneProject(
                    Velocity, CurrentFloor.HitResult.Normal);
                Velocity = SlideDir;
            }
        }
    }

    DebugLogMovement(TEXT("PhysFalling"), VelBefore, Velocity);
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

void UAC_CharacterMovementBase::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
    if (Hit.GetActor() && Hit.GetActor()->IsA<ACharacter>())
    {
        UE_LOG(LogTemp, Error, TEXT("🔴 LANDED ON CHARACTER - BLOCKING!"));

        // Floor'u temizle - ÇOK ÖNEMLİ!
        CurrentFloor.Clear();

        // Falling mode'a geç
        SetMovementMode(MOVE_Falling);

        // Velocity'yi düzelt - capsule'dan kayarak geç
        FVector SlideDirection = FVector::VectorPlaneProject(Velocity, Hit.Normal);
        SlideDirection.Z = FMath::Min(SlideDirection.Z, -100.f); // Aşağı momentum ekle

        Velocity = SlideDirection;

        // Karakteri hit noktasından hafif uzaklaştır
        FVector AwayFromHit = -Hit.Normal * 2.0f; // 2cm uzaklaş
        FHitResult MoveHit;
        SafeMoveUpdatedComponent(AwayFromHit, UpdatedComponent->GetComponentQuat(),
            true, MoveHit);

        return; // ProcessLanded'i SKIP et!
    }

    Super::ProcessLanded(Hit, remainingTime, Iterations);
}

void UAC_CharacterMovementBase::SetPostLandedPhysics(const FHitResult& Hit)
{
    FVector VelBefore = Velocity;

    UE_LOG(LogTemp, Error, TEXT("🔴 SetPostLandedPhysics CALLED"));

    if (Hit.GetActor() && Hit.GetActor()->IsA<ACharacter>())
    {
        UE_LOG(LogTemp, Error, TEXT("🔴 BLOCKING SetPostLandedPhysics on Character!"));
        return; // Landing physics'i SKIP et!
    }

    Super::SetPostLandedPhysics(Hit);

    DebugLogMovement(TEXT("SetPostLandedPhysics"), VelBefore, Velocity, &Hit);
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

/*
void UAC_CharacterMovementBase::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{

    UE_LOG(LogTemp, Error, TEXT("🔴 MoveUpdatedComponent - Hit Character!"));
    UE_LOG(LogTemp, Error, TEXT("   Normal: %s"), *OutHit->Normal.ToString());
    UE_LOG(LogTemp, Error, TEXT("   Impact: %s"), *OutHit->ImpactPoint.ToString());

    DebugLogMovement(TEXT("MoveUpdatedComponent"), VelBefore, Velocity, OutHit);

    // Burada velocity düzeltmesi yap
    if (Velocity.Z > 0.f && OutHit->Normal.Z < 0.f)
    {
        // Yukarı zıplama var - bunu sıfırla!
        UE_LOG(LogTemp, Error, TEXT("🔴 FIXING BOUNCE!"));
        Velocity.Z = FMath::Min(Velocity.Z, 0.f);
    }

}
*/
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
