// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Actors/Characters/Heroes/Components/Listener/AC_HeroEnemyAttackListener.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Animation/ANS_AttackTrace.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include <Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h>

UAC_HeroEnemyAttackListener::UAC_HeroEnemyAttackListener()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroEnemyAttackListener::BeginPlay()
{
    Super::BeginPlay();

    US_SpawnDelegates* SpawnSubs = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>();
    if (!SpawnSubs)
    {
        return;
    }

    SpawnSubs->OnEnemySpawn.AddDynamic(this, &UAC_HeroEnemyAttackListener::OnEnemySpawn);
    for (const FEnemySpawnData& ExistingEnemy : SpawnSubs->AliveEnemies)
    {
        OnEnemySpawn(ExistingEnemy);
    }

    checkf(HeroASC, TEXT("HeroASC is null in %s"), *GetClass()->GetName());
    checkf(PerfectWindowTagEffectClass, TEXT("PerfectWindowTagEffectClass is null in %s"), *GetClass()->GetName());
}

void UAC_HeroEnemyAttackListener::OnEnemySpawn(const FEnemySpawnData& EnemySpawnData)
{
    if (!EnemySpawnData.Character || !EnemySpawnData.ASC)
    {
        return;
    }

    // ASC callback ekle ve weak pointer ile takip et
    EnemySpawnData.ASC->AbilityActivatedCallbacks.AddUObject(this, &UAC_HeroEnemyAttackListener::OnEnemyAbilityActivated);
}

void UAC_HeroEnemyAttackListener::OnEnemyAbilityActivated(UGameplayAbility* Ability)
{
    if (!Ability || !PerfectWindowTagEffectClass || !HeroASC)
    {
        return;
    }

    UGA_MeleeAttackBase* MeleeAttack = Cast<UGA_MeleeAttackBase>(Ability);
    if (!MeleeAttack || !MeleeAttack->AnimMontage)
    {
        return;
    }

    const float AttackTime = GetAttackNotifyTriggerTime(MeleeAttack);
    if (AttackTime < 0.f)
    {
        return;
    }

    const float ApplyDelay = FMath::Max(AttackTime - PerfectWindowEarlyOffset, 0.f);

    FTimerHandle TimerHandle;
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            TimerHandle,
            this,
            &UAC_HeroEnemyAttackListener::ApplyPerfectWindowTagEffect,
            ApplyDelay,
            false
        );
    }

    PendingPerfectWindowTimers.Add(TimerHandle);
}

float UAC_HeroEnemyAttackListener::GetAttackNotifyTriggerTime(UGA_MeleeAttackBase* Ability)
{
    if (!Ability || !Ability->AnimMontage)
    {
        return -1.0f;
    }

    const UAnimMontage* Montage = Ability->AnimMontage;
    float NotifyStartTime = -1.0f;

    for (const FAnimNotifyEvent& NotifyEvent : Montage->Notifies)
    {
        if (NotifyEvent.NotifyStateClass && NotifyEvent.NotifyStateClass->IsA<UANS_AttackTrace>())
        {
            const UANS_AttackTrace* StateNotify = Cast<UANS_AttackTrace>(NotifyEvent.NotifyStateClass);
            if (StateNotify->EventTagStart == GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_Start)
            {
                NotifyStartTime = NotifyEvent.GetTime(); // NotifyBegin zamanı
                break;
            }
        }
    }

    return NotifyStartTime >= 0.f ? NotifyStartTime : -1.f;
}

void UAC_HeroEnemyAttackListener::ApplyPerfectWindowTagEffect()
{
    if (!PerfectWindowTagEffectClass)
    {
        return;
    }

    UGameplayEffect* PerfectWindowTagEffect = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(PerfectWindowTagEffectClass);
    if (!PerfectWindowTagEffect) 
    {
        return;
    }

    HeroASC->ApplyGameplayEffectToSelf(PerfectWindowTagEffect, 0, FGameplayEffectContextHandle());
}

void UAC_HeroEnemyAttackListener::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (UWorld* World = GetWorld())
    {
        for (FTimerHandle& Handle : PendingPerfectWindowTimers)
        {
            World->GetTimerManager().ClearTimer(Handle);
        }
    }

    PendingPerfectWindowTimers.Empty();

    if (US_SpawnDelegates* SpawnSubs = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnSubs->OnEnemySpawn.RemoveDynamic(this, &UAC_HeroEnemyAttackListener::OnEnemySpawn);
    }
}