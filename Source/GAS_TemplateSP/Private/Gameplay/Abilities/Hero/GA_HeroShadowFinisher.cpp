// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroShadowFinisher.h"
#include "Gameplay/Abilities/TargetActors/Shadows/HeroShadowTargetActor.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"

UGA_HeroShadowFinisher::UGA_HeroShadowFinisher()
{
    ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher);
}

bool UGA_HeroShadowFinisher::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    // First, run the default checks (cooldown, blocking tags, etc.)
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    // Prevent activation if the hero is not currently target locked.
    // This ability requires the hero to be in a target lock state to execute.
    if (ActorInfo->AbilitySystemComponent.IsValid())
    {
        if (!ActorInfo->AbilitySystemComponent.Get()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
        {
            return false;
        }
    }

    return true;
}

void UGA_HeroShadowFinisher::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
    AHeroShadowTargetActor* HeroShadowTargetActor = Cast<AHeroShadowTargetActor>(TargetActorData.TargetActor);
    if (!HeroShadowTargetActor)
    {
        Super::OnTargetActorConfirm(TargetActorData);
    }

    BP_OnTargetActorConfirm(TargetActorData);

    GetAvatarActorFromActorInfo()->SetActorLocation(HeroShadowTargetActor->GetActorLocation());
    GetAvatarActorFromActorInfo()->SetActorRotation(HeroShadowTargetActor->GetActorRotation());

    if (HeroShadowTargetActor->GetSelectedShadowAbilityClass() && HeroShadowTargetActor->GetCurrentTarget())
    {
        if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher))
        {
            GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher);
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

void UGA_HeroShadowFinisher::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{ 
    // Adjust the rotation towards the target enemy before spawning the target actor.
    if (AGAS_HeroBase* OwnerHero = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo()))
    {
        if (UAC_TargetLockSystem* TargetLockSystemComponent = OwnerHero->GetTargetLockSystemComponent())
        {
            if (AActor* CurrentTarget = TargetLockSystemComponent->CurrentTarget) 
            {
                FRotator LookAtToTargetRotation = UKismetMathLibrary::FindLookAtRotation(Location, CurrentTarget->GetActorLocation());
                Super::SpawnAndSetupTargetActor(LookAtToTargetRotation, Location);
                return;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Couldn't set rotation correctly for Finisher Shadow in: %s"), *GetName());
    Super::SpawnAndSetupTargetActor(Rotation, Location);
}

void UGA_HeroShadowFinisher::OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
    Super::OnTargetActorSpawnLocationQueryFinished(Result);
}

void UGA_HeroShadowFinisher::CancelAbilityFromInput()
{
    
}
