// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroHologram.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/SC_HeroHologramController.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UGA_HeroHologram::UGA_HeroHologram()
{
    ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram);
}

void UGA_HeroHologram::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    const FGameplayEventData* TriggerEventData)
{
    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        //bActorWillSpawnWithEQS = false;
    }
    Super::ActivateAbility (Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_HeroHologram::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
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
        if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram))
        {
            GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram);
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

void UGA_HeroHologram::OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
    Super::OnTargetActorSpawnLocationQueryFinished(Result);

    if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo()))
    {
        USC_HeroHologramController* HeroHologramController = HeroBase->GetHeroHologramControllerComponent();
        HeroHologramController->HeroHologramTargetActor = Cast<AHeroHologramTargetActor>(TargetActor);
    }
}

void UGA_HeroHologram::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
    AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!HeroBase || !TraceData) 
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase or TraceData is null in: %s"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
    }

    FVector HologramSpawnLocation = Location;

    if (HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        AActor* LockedTarget = HeroBase->GetTargetLockSystemComponent()->CurrentTarget;
        HeroBase->GetHeroHologramControllerComponent()->ResetRightTraceDistance();
        FVector HeroLocatoin = HeroBase->GetActorLocation();
        float DistanceBetweenHologramAndHero = FVector::Dist(HologramSpawnLocation, HeroLocatoin);
        float DistanceBetweenHologramAndTarget = FVector::Dist(LockedTarget->GetActorLocation(), HologramSpawnLocation);
        FVector2D Axises = FVector2D(DistanceBetweenHologramAndTarget, DistanceBetweenHologramAndHero);
        HeroBase->GetHeroHologramControllerComponent()->SetCumulativeMouseValuesRelatedWith2DLocation(Axises);
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HologramSpawnLocation, LockedTarget->GetActorLocation());
        Super::SpawnAndSetupTargetActor(FRotator(0, LookAtRotation.Yaw, 0), HologramSpawnLocation);
    }
    else
    {
        HologramSpawnLocation = HeroBase->GetHeroHologramControllerComponent()->GetHeroHologramLocationFromLineTrace();
        TArray<AActor*> OutResultActors;
        TraceData->Trace->CreateTraceWithTeamFilterAndLocation(GetWorld(), HeroBase, ETeamAttitude::Hostile, HologramSpawnLocation, OutResultActors);

        if (OutResultActors.IsValidIndex(0))
        {
            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HologramSpawnLocation, OutResultActors[0]->GetActorLocation());
            Super::SpawnAndSetupTargetActor(FRotator(0, LookAtRotation.Yaw, 0), HologramSpawnLocation);
        }
        else
        {
            Super::SpawnAndSetupTargetActor(HeroBase->GetActorRotation(), HologramSpawnLocation);
        }
    }
}


