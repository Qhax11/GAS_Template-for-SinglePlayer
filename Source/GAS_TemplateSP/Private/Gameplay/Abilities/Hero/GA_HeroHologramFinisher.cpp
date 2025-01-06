// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroHologramFinisher.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"

void UGA_HeroHologramFinisher::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
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
        if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram))
        {
            GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram);
        }

        TargetActorData.TargetActor->Destroy();
        Super::OnTargetActorConfirm(TargetActorData);
    }
    else
    {
        Super::OnTargetActorConfirm(TargetActorData);
    }
}

void UGA_HeroHologramFinisher::OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
    Super::OnTargetActorSpawnLocationQueryFinished(Result);

}
