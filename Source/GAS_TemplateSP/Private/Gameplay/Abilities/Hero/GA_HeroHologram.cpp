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
        FVector CurrentTargetLocation = HeroBase->GetTargetLockSystemComponent()->CurrentTarget->GetActorLocation();

        HeroBase->GetHeroHologramControllerComponent()->ResetRightTraceDistance();
        FVector HeroLocatoin = HeroBase->GetActorLocation();

        float DistanceBetweenHologramAndHero = FVector::Dist(HologramSpawnLocation, HeroLocatoin);
        float DistanceBetweenHologramAndTarget = GetPointDistToLine(HologramSpawnLocation, CurrentTargetLocation);


        FVector2D Axes = FVector2D(DistanceBetweenHologramAndTarget, DistanceBetweenHologramAndHero);
        HeroBase->GetHeroHologramControllerComponent()->SetCumulativeMouseValuesRelatedWith2DLocation(Axes);

        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HologramSpawnLocation, CurrentTargetLocation);
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

    SetHologramToHologramController();
}

void UGA_HeroHologram::SetHologramToHologramController()
{
    if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo()))
    {
        USC_HeroHologramController* HeroHologramController = HeroBase->GetHeroHologramControllerComponent();
        HeroHologramController->HeroHologramTargetActor = Cast<AHeroHologramTargetActor>(TargetActor);
    }
}

float UGA_HeroHologram::GetPointDistToLine(FVector HologramSpawnLocation, FVector CurrentTargetLocation)
{
    FVector HeroLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

    // Adjust the target's Z coordinate to match the hero's Z coordinate, focusing only on the XY plane for direction calculation.
    // This effectively ignores the Z-axis difference, providing a direction vector confined to the horizontal plane.
    FVector HeroLocationUpdated = FVector(HeroLocation.X, HeroLocation.Y, CurrentTargetLocation.Z);
    FVector HeroHologramLocationUpdated = FVector(HologramSpawnLocation.X, HologramSpawnLocation.Y, CurrentTargetLocation.Z);

    FVector NormalizedDirection = HeroLocationUpdated - CurrentTargetLocation;
    NormalizedDirection.Normalize();

    FVector ClosestPointOnLine;
    float PointDistToLine = FMath::PointDistToLine(HeroHologramLocationUpdated, NormalizedDirection, HeroLocationUpdated, ClosestPointOnLine);

    if (bDrawDebug)
    {
        DrawDebugLine(GetWorld(), HeroLocationUpdated, CurrentTargetLocation, FColor::Red, false, 10.0f, 0, 2.0f);
        DrawDebugLine(GetWorld(), ClosestPointOnLine, HeroHologramLocationUpdated, FColor::Blue, false, 10.0f, 0, 2.0f);
    }

    // Determines whether the hologram is to the right or left of a line defined by the normalized direction. 
    // Uses the cross product to check the Z-axis value: 
    // - If Z <= 0, the hologram is on the left side or on the line.
    // - If Z > 0, the hologram is on the right side.
    // Returns the distance to the line with a positive or negative sign based on the side.
    FVector PointDirection = HologramSpawnLocation - HeroLocation;
    FVector CrossProductResult = FVector::CrossProduct(NormalizedDirection, PointDirection);

    if (CrossProductResult.Z <= 0)
    {
        return PointDistToLine;
    }
    else
    {
        return -PointDistToLine;
    }
}


