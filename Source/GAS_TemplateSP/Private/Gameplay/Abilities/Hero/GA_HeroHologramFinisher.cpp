// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroHologramFinisher.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"

UGA_HeroHologramFinisher::UGA_HeroHologramFinisher()
{
    ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_CanActivateFinisher);
}

void UGA_HeroHologramFinisher::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
    AHeroHologramTargetActor* HeroHologramTargetActor = Cast<AHeroHologramTargetActor>(TargetActorData.TargetActor);
    if (!HeroHologramTargetActor)
    {
        Super::OnTargetActorConfirm(TargetActorData);
    }

    BP_OnTargetActorConfirm(TargetActorData);

    GetAvatarActorFromActorInfo()->SetActorLocation(HeroHologramTargetActor->GetActorLocation());
    GetAvatarActorFromActorInfo()->SetActorRotation(HeroHologramTargetActor->GetActorRotation());

    if (HeroHologramTargetActor->AttackMontage && HeroHologramTargetActor->CurrentTarget)
    {
        if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_CanActivateFinisher))
        {
            GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_CanActivateFinisher);
        }

        if (GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(FinisherAbilityClass)) 
        {

        }

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
