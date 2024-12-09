// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroHologram.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGA_HeroHologram::OnGameplayEventValidData(const FGameplayAbilityTargetDataHandle& Data)
{
    if (!Data.IsValid(0))
    {
        return;
    }

    TArray<AActor*> Actors = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(Data, 0);
    if (Actors.IsValidIndex(0))
    {
        AHeroHologramTargetActor* HeroHologramTargetActor = Cast<AHeroHologramTargetActor>(Actors[0]);
        GetAvatarActorFromActorInfo()->SetActorLocation(HeroHologramTargetActor->GetActorLocation());
        GetAvatarActorFromActorInfo()->SetActorRotation(HeroHologramTargetActor->GetActorRotation());

        if (HeroHologramTargetActor->AttackMontage && HeroHologramTargetActor->CurrentTarget)
        {
            CreatePlayMontageWaitForEvent(HeroHologramTargetActor->AttackMontage, FName("Section2"));
        }
        else
        {
            // Calling EndAbility
            Super::OnGameplayEventValidData(Data);
        }
    }
}
