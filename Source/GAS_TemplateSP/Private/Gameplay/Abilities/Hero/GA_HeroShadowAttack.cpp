// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroShadowAttack.h"
#include "Gameplay/Abilities/TargetActors/Shadows/HeroShadowTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/SC_HeroShadowController.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UGA_HeroShadowAttack::UGA_HeroShadowAttack()
{
    ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Shadow);
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
    }
   
    BP_OnTargetActorConfirm(TargetActorData);

    GetAvatarActorFromActorInfo()->SetActorLocation(HeroShadowTargetActor->GetActorLocation());
    GetAvatarActorFromActorInfo()->SetActorRotation(HeroShadowTargetActor->GetActorRotation());

    if (HeroShadowTargetActor->GetSelectedShadowAbility() && HeroShadowTargetActor->GetCurrentTarget())
    {
        if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Shadow))
        {
            GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Shadow);
        }
        if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo()))
        {
            HeroBase->GetHeroMeleeComboManagerComponent()->ActivateComboMeleeAttackAbility(FName("Section2"));
        }
        Super::OnTargetActorConfirm(TargetActorData);
    }
    else
    {
        Super::OnTargetActorConfirm(TargetActorData);
    }
}

void UGA_HeroShadowAttack::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
    AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!HeroBase || !TraceData) 
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase or TraceData is null in: %s"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
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

        TArray<AActor*> OutResultActors;
        TraceData->Trace->CreateTraceWithTeamFilterWithLocation(GetWorld(), HeroBase, ETeamAttitude::Hostile, ShadowSpawnLocation, OutResultActors);

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
        USC_HeroShadowController* HeroShadowController = HeroBase->GetHeroShadowControllerComponent();
        HeroShadowController->HeroShadow = Cast<AHeroShadowTargetActor>(TargetActor);
    }
}




