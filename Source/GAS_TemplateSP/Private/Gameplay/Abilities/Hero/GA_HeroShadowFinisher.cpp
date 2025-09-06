// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroShadowFinisher.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/TargetActors/Shadows/HeroShadowTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Kismet/KismetMathLibrary.h"

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

void UGA_HeroShadowFinisher::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    FGameplayTag SelectedFinisherTag = GetRandomFinisherTag();
    UAnimMontage* SelectedFinisherMontage = DA_FinisherAttackMontage->FindMontageByTag(SelectedFinisherTag);

    AHeroShadowTargetActor* HeroShadowTargetActor = Cast<AHeroShadowTargetActor>(TargetActor);
    if (!HeroShadowTargetActor) 
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroShadowTargetActor is null!"));
        return;
    }
}

FGameplayTag UGA_HeroShadowFinisher::GetRandomFinisherTag()
{
    int32 RandomIndex = FMath::RandRange(0, 2);

    switch (RandomIndex)
    {
    case 0:
        return GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_1;
    case 1:
        return GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_2;
    default:
        return GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeFinisher_3;
    }
}

void UGA_HeroShadowFinisher::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
    AGAS_HeroBase* OwnerHero = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!OwnerHero)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerHero is null in: %s, cannot initalize the ability"), *GetName());
        return;
    }

    UAC_TargetLockSystem* TargetLockSystemComponent = OwnerHero->GetTargetLockSystemComponent();
    if (!TargetLockSystemComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetLockSystemComponent is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    AGAS_CharacterBase* CurrentEnemyTargetCharacter = Cast<AGAS_CharacterBase>(TargetLockSystemComponent->CurrentTarget);
    if (!CurrentEnemyTargetCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentTarget is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    UAC_TagDelegates* EnemyTargetTageDeleagtesComp = CurrentEnemyTargetCharacter->GetTagDelegatesComponent();
    if (!EnemyTargetTageDeleagtesComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetTageDeleagtesComp is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    EnemyTargetTageDeleagtesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable, EListenMode::OnRemoved).BindDynamic(this, &UGA_HeroShadowFinisher::OnEnemyTargetVulnerableTagRemoved);

    UAbilitySystemComponent* CurrentTargetASC = TargetLockSystemComponent->CurrentTargetASC;
    if (!CurrentTargetASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentTargetASC is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    FRotator LookAtToTargetRotation = UKismetMathLibrary::FindLookAtRotation(Location, CurrentEnemyTargetCharacter->GetActorLocation());

    Super::SpawnAndSetupTargetActor(Rotation, Location);
}

void UGA_HeroShadowFinisher::OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
    Super::OnTargetActorSpawnLocationQueryFinished(Result);
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

    if (!HeroShadowTargetActor->GetSelectedAttackAbilityClass() || !HeroShadowTargetActor->GetCurrentTarget())
    {
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher))
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher);
    }

    GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(HeroShadowTargetActor->GetSelectedAttackAbilityClass());

    Super::OnTargetActorConfirm(TargetActorData);
}

void UGA_HeroShadowFinisher::OnEnemyTargetVulnerableTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    if (TargetActor->IsValidLowLevel())
    {
        TargetActor->Cancel();
    }
}

void UGA_HeroShadowFinisher::CancelAbilityFromInput()
{
    // We don't call Super:CancelAbilityFromInput(); 
    // This ability should cancel on only OnEnemyTargetVulnerableTagRemoved
}

