// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Actors/Characters/Heroes/Components/Listener/AC_HeroEnemyAttackListener.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Animation/AN_SendTag.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"

UAC_HeroEnemyAttackListener::UAC_HeroEnemyAttackListener()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroEnemyAttackListener::BeginPlay()
{
    Super::BeginPlay();

    US_SpawnDelegates* SpawnSubs = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>();
    if (!SpawnSubs) return;

    // 1. Gelecek için abone ol
    SpawnSubs->OnEnemySpawn.AddDynamic(this, &UAC_HeroEnemyAttackListener::OnEnemySpawn);

    // 2. Geçmişte doğmuş olanları işle
    for (const FEnemySpawnData& ExistingEnemy : SpawnSubs->AliveEnemies)
    {
        OnEnemySpawn(ExistingEnemy);
    }
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
    if (!Ability)
    {
        return;
    }

    UGA_MeleeAttackBase* MeleeAttackAbility = Cast<UGA_MeleeAttackBase>(Ability);
    if (!MeleeAttackAbility)
    {
        return;
    }

    UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(GetOwner()->FindComponentByClass<UAbilitySystemComponent>());
    if (!ASC)
    {
        return;
    }

    // Ability taglerini al
    FGameplayTagContainer CombinedTags;
    CombinedTags.AppendTags(Ability->GetAssetTags());
    if (const FGameplayAbilitySpec* Spec = Ability->GetCurrentAbilitySpec())
    {
        CombinedTags.AppendTags(Spec->DynamicAbilityTags);
    }

    float AttackTime = GetAttackNotifyTriggerTime(MeleeAttackAbility, CombinedTags);
    if (AttackTime < 0.0f)
    {
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    float PerfectWindowStartTime = CurrentTime + (AttackTime - PerfectOffsetStart);
    float PerfectWindowEndTime = CurrentTime + (AttackTime + PerfectOffsetEnd);

    // Timer lambda’larında weak pointer kullan
    TWeakObjectPtr<UAbilitySystemComponent> ASCWeak = ASC;

    FTimerDelegate TimerStartDelegate;
    TimerStartDelegate.BindLambda([ASCWeak]()
        {
            if (ASCWeak.IsValid())
            {
                ASCWeak->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect);
            }
        });

    FTimerDelegate TimerEndDelegate;
    TimerEndDelegate.BindLambda([ASCWeak]()
        {
            if (ASCWeak.IsValid())
            {
                ASCWeak->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect);
            }
        });

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Start, TimerStartDelegate, AttackTime - PerfectOffsetStart, false);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_End, TimerEndDelegate, AttackTime + PerfectOffsetEnd, false);
}

float UAC_HeroEnemyAttackListener::GetAttackNotifyTriggerTime(UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags)
{
    if (!Ability || !Ability->AnimMontage)
    {
        return -1.0f;
    }

    const UAnimMontage* Montage = Ability->AnimMontage;

    float NotifyTime = -1.0f;
    for (const FAnimNotifyEvent& Notify : Montage->Notifies)
    {
        if (const UAN_SendTag* TagNotify = Cast<UAN_SendTag>(Notify.Notify))
        {
            if (TagNotify->NotifyTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Attack_TraceStart)
            {
                NotifyTime = Notify.GetTriggerTime();
                break;
            }
        }
    }

    return NotifyTime >= 0.f ? NotifyTime : -1.f;
}

void UAC_HeroEnemyAttackListener::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (US_SpawnDelegates* SpawnSubs = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnSubs->OnEnemySpawn.RemoveDynamic(this, &UAC_HeroEnemyAttackListener::OnEnemySpawn);
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Start);
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle_End);
    }
}