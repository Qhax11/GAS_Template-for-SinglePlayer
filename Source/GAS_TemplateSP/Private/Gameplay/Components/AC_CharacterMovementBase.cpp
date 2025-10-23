// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_CharacterMovementBase.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include <AbilitySystemGlobals.h>

void UAC_CharacterMovementBase::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
    AActor* HitActor = Hit.GetActor();
    if (!HitActor)
    {
        Super::HandleImpact(Hit, TimeSlice, MoveDelta);
        return;
    }

    UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);
    if (!TargetASC || !TargetASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Enemy))
    {
        Super::HandleImpact(Hit, TimeSlice, MoveDelta);
        return;
    }

    // Çarpma noktasýnýn yüksekliðini kontrol et
    float HitHeight = Hit.ImpactPoint.Z - HitActor->GetActorLocation().Z;

    if (HitHeight > 50.0f) // Üstten çarpýyorsa
    {
        // Normal'in yönüne bak - yan taraftan mý çarpýyor üstten mi?
        if (Hit.Normal.Z < -0.7f) // Üstten aþaðý bakan normal (taban gibi)
        {
            // Tamamen normal floor gibi davran - durabilsin üstünde
            Super::HandleImpact(Hit, TimeSlice, MoveDelta);
            return;
        }

        // Yan veya eðik yüzey - KAYSIN!
        FVector CurrentVel = Velocity;

        // Yatay momentum'u TAM koru
        Velocity.X = CurrentVel.X;
        Velocity.Y = CurrentVel.Y;

        // Dikey hýzý sýfýrdan küçükse koru (düþüyor), büyükse sýfýrla (zýplýyor)
        Velocity.Z = FMath::Min(CurrentVel.Z, 0.0f);

        // Kayma efekti için hit normal'ine göre hafif bir itme ekle
        FVector SlideDirection = FVector::VectorPlaneProject(MoveDelta.GetSafeNormal(), Hit.Normal);
        if (!SlideDirection.IsNearlyZero())
        {
            SlideDirection.Z = FMath::Min(SlideDirection.Z, 0.0f); // Sadece aþaðý veya yatay
            Velocity += SlideDirection * 100.0f; // Kayma hýzýný artýr
        }

        return; // Super çaðýrma - fiziksel tepki yok!
    }

    // Normal impact handling (altta veya yandan çarpma)
    Super::HandleImpact(Hit, TimeSlice, MoveDelta);
}
