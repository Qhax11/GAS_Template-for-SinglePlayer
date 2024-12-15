// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroHologram.h"
#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/SC_HologramAbilityHelper.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_HeroHologram::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo()))
    {
        USC_HologramAbilityHelper* HologramAbilityHelper = HeroBase->GetHologramAbilityHelperComponent();
        HologramAbilityHelper->HeroHologramTargetActor = Cast<AHeroHologramTargetActor>(TargetActor);
    }
}

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
    AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!HeroBase || !TraceData) 
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase or TraceData is null in: %s"), *GetName());
        return Super::SpawnAndSetupTargetActor(Rotation, Location);
    }

    FVector HologramStartLocation = HeroBase->GetHologramAbilityHelperComponent()->GetHeroHologramLocationFromLineTrace();
    TArray<AActor*> OutResultActors;
    TraceData->Trace->CreateTraceWithTeamFilterAndLocation(GetWorld(), HeroBase, ETeamAttitude::Hostile, HologramStartLocation, OutResultActors);

    if (OutResultActors.IsValidIndex(0)) 
    {
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HologramStartLocation, OutResultActors[0]->GetActorLocation());
        return Super::SpawnAndSetupTargetActor(FRotator(0, LookAtRotation.Yaw, 0), HologramStartLocation);
    }
    else
    {
        return Super::SpawnAndSetupTargetActor(HeroBase->GetActorRotation(), HologramStartLocation);
    }
}
