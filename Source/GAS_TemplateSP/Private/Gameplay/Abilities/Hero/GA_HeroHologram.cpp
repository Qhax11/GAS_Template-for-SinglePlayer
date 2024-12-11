// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroHologram.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGA_HeroHologram::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
    AHeroHologramTargetActor* HeroHologramTargetActor = Cast<AHeroHologramTargetActor>(TargetActorData.TargetActor);
    if (!HeroHologramTargetActor)
    {
        Super::OnTargetActorConfirm(TargetActorData);
    }

    GetAvatarActorFromActorInfo()->SetActorLocation(HeroHologramTargetActor->GetActorLocation());
    GetAvatarActorFromActorInfo()->SetActorRotation(HeroHologramTargetActor->GetActorRotation());
    
    if (HeroHologramTargetActor->AttackMontage && HeroHologramTargetActor->CurrentTarget)
    {
        CreatePlayMontageWaitForEvent(HeroHologramTargetActor->AttackMontage);
    }
   
    Super::OnTargetActorConfirm(TargetActorData);
}

AGAS_TargetActorBase* UGA_HeroHologram::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
    return Super::SpawnAndSetupTargetActor(Rotation, Location);
}
