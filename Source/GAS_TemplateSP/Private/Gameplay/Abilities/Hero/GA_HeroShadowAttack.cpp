// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroShadowAttack.h"
#include "Gameplay/Actors/Characters/Heroes/Components/SC_HeroShadowController.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Abilities/TargetActors/Shadows/HeroShadowTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Kismet/KismetMathLibrary.h"

UGA_HeroShadowAttack::UGA_HeroShadowAttack()
{
    AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Attack_Shadow);
    ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Shadow);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher);
}

void UGA_HeroShadowAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility (Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_HeroShadowAttack::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
    AHeroShadowTargetActor* HeroShadowTargetActor = Cast<AHeroShadowTargetActor>(TargetActorData.TargetActor);
    if (!HeroShadowTargetActor)
    {
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    GetAvatarActorFromActorInfo()->SetActorLocation(HeroShadowTargetActor->GetActorLocation());
    GetAvatarActorFromActorInfo()->SetActorRotation(HeroShadowTargetActor->GetActorRotation());

    // Temporarily remove the targeting state tag to prevent HeroComboManager from responding to input while this ability is active.
    // The tag will be re-applied automatically when the ability ends, but we need it removed earlier for proper input blocking.
    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Shadow))
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Shadow);
    }

    if (!TargetActorData.AbilityClass || !HeroShadowTargetActor->GetCurrentTarget())
    {
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo()))
    {
        USC_HeroShadowController* HeroShadowController = HeroBase->GetHeroShadowControllerComponent();
        HeroShadowController->OnShadowAttackConfirmed.Broadcast();
    }

    // Triggers the actual attack abilities from BP side.
    BP_OnTargetActorConfirm(TargetActorData);

    Super::OnTargetActorConfirm(TargetActorData);
}

void UGA_HeroShadowAttack::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
    AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!HeroBase || !TraceData) 
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase or TraceData is null in: %s"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    FVector ShadowSpawnLocation = Location;

    if (HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        FVector CurrentTargetLocation = HeroBase->GetTargetLockSystemComponent()->CurrentTarget->GetActorLocation();
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ShadowSpawnLocation, CurrentTargetLocation);
        Super::SpawnAndSetupTargetActor(FRotator(0, LookAtRotation.Yaw, 0), ShadowSpawnLocation);

        SetShadowToShadowController();
        HeroBase->GetHeroShadowControllerComponent()->SetShadowLocationWithCumulativeMouseValuesTargetLocked();
    }
    else
    {
        ShadowSpawnLocation = HeroBase->GetHeroShadowControllerComponent()->GetHeroShadowLocationFromLineTrace();
        ShadowSpawnLocation.Z += 90;

        TArray<AActor*> OutResultActors;
        FTraceRequest TraceRequest;
        TraceRequest.StartLocation = ShadowSpawnLocation;
        TraceData->Trace->CreateTraceWithTeamFilter(GetWorld(), HeroBase, ETeamAttitude::Hostile, OutResultActors, TraceRequest);

        if (OutResultActors.IsValidIndex(0))
        {
            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ShadowSpawnLocation, OutResultActors[0]->GetActorLocation());
            Super::SpawnAndSetupTargetActor(FRotator(0, LookAtRotation.Yaw, 0), ShadowSpawnLocation);
        }
        else
        {
            Super::SpawnAndSetupTargetActor(HeroBase->GetActorRotation(), ShadowSpawnLocation);
        }
        SetShadowToShadowController();
    }
}

void UGA_HeroShadowAttack::SetShadowToShadowController()
{
    if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo()))
    {
        if (USC_HeroShadowController* HeroShadowController = HeroBase->GetHeroShadowControllerComponent())
        {
            HeroShadowController->SetHeroShadowActor(Cast<AHeroShadowTargetActor>(TargetActor));
        }
    }
}




