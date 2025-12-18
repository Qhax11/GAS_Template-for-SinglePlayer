// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroShadowFinisher.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/TargetActors/Shadows/HeroShadowTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemGlobals.h"

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

    AHeroShadowTargetActor* HeroShadowTargetActor = Cast<AHeroShadowTargetActor>(TargetActor);
    if (!HeroShadowTargetActor) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: HeroShadowTargetActor is null!"));
        return;
    }
}

void UGA_HeroShadowFinisher::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
    AGAS_HeroBase* OwnerHero = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!OwnerHero)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: OwnerHero is null in: %s, cannot initalize the ability"), *GetName());
        return;
    }

    UAC_TargetLockSystem* TargetLockSystemComponent = OwnerHero->GetTargetLockSystemComponent();
    if (!TargetLockSystemComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: TargetLockSystemComponent is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    AGAS_CharacterBase* CurrentEnemyTargetCharacter = Cast<AGAS_CharacterBase>(TargetLockSystemComponent->CurrentTarget);
    if (!CurrentEnemyTargetCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: CurrentTarget is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    UAC_TagDelegates* EnemyTargetTageDeleagtesComp = CurrentEnemyTargetCharacter->GetTagDelegatesComponent();
    if (!EnemyTargetTageDeleagtesComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: TargetTageDeleagtesComp is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    EnemyTargetTageDeleagtesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable, EListenMode::OnRemoved).BindDynamic(this, &UGA_HeroShadowFinisher::OnEnemyTargetVulnerableTagRemoved);

    UAbilitySystemComponent* CurrentTargetASC = TargetLockSystemComponent->CurrentTargetASC;
    if (!CurrentTargetASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: CurrentTargetASC is null in: %s, cannot initalize the ability"), *GetName());
        Super::SpawnAndSetupTargetActor(Rotation, Location);
        return;
    }

    FRotator LookAtToTargetRotation = UKismetMathLibrary::FindLookAtRotation(Location, CurrentEnemyTargetCharacter->GetActorLocation());

    Super::SpawnAndSetupTargetActor(LookAtToTargetRotation, Location);
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
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: HeroShadowTargetActor is null in %s, cannot initialize ShadowFinisher."), *GetName());
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    TSubclassOf<UGA_MeleeAttackBase> MeleeFinisherClass = HeroShadowTargetActor->GetSelectedAttackAbilityClass();
    if (!MeleeFinisherClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: MeleeFinisherClass is null in %s, cannot initialize ShadowFinisher."), *GetName());
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    AActor* CurrentTarget = HeroShadowTargetActor->GetCurrentTarget();
    if (!CurrentTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: CurrentTarget is null in %s, cannot initialize ShadowFinisher."), *GetName());
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    UAbilitySystemComponent* TargetEnemyASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CurrentTarget);
    if (!TargetEnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: TargetEnemyASC is null for actor: %s in %s, cannot initialize ShadowFinisher."),
            *CurrentTarget->GetName(), *GetName());
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    UGAS_AbilitySystemComponent* EnemyASC = Cast<UGAS_AbilitySystemComponent>(TargetEnemyASC);
    if (!EnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: Failed to cast ASC to UGAS_AbilitySystemComponent for actor: %s in %s"),
            *CurrentTarget->GetName(), *GetName());
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    BP_OnTargetActorConfirm(TargetActorData);

    GetAvatarActorFromActorInfo()->SetActorLocation(HeroShadowTargetActor->GetActorLocation());
    GetAvatarActorFromActorInfo()->SetActorRotation(HeroShadowTargetActor->GetActorRotation());

    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher))
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanActivateFinisher);
    }

    UGA_MeleeFinisher* ActivatedMeleeFinisher = Cast<UGA_MeleeFinisher>(GetASC()->TryActivateAbilityByClassAndReturnInstance(MeleeFinisherClass));
    if (!ActivatedMeleeFinisher)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroShadowFinisher: ActivatedMeleeFinisher is null in %s, cannot initialize ShadowFinisher."), *GetName());
        Super::OnTargetActorConfirm(TargetActorData);
        return;
    }

    FGameplayEventData EnemyDeathFinisher;
    EnemyDeathFinisher.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_Death_Finisher;
    FGameplayEffectContextHandle GE_ContextHandleForDeathFinisher = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    GE_ContextHandleForDeathFinisher.SetAbility(ActivatedMeleeFinisher);
    EnemyDeathFinisher.ContextHandle = GE_ContextHandleForDeathFinisher;
    EnemyDeathFinisher.Instigator = GetAvatarActorFromActorInfo();

    EnemyASC->TryActivateAbilityByEventData(EnemyDeathFinisher);

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

