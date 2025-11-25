// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Actors/Characters/Heroes/Components/Listener/AC_HeroEnemyAttackListener.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Animation/AN_SendGameplayEvent.h"
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
    if (!SpawnSubs)
    {
        return;
    }

    SpawnSubs->OnEnemySpawn.AddDynamic(this, &UAC_HeroEnemyAttackListener::OnEnemySpawn);
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

    FGameplayTagContainer CombinedTags;
    CombinedTags.AppendTags(Ability->GetAssetTags());
    if (const FGameplayAbilitySpec* Spec = Ability->GetCurrentAbilitySpec())
    {
        CombinedTags.AppendTags(Spec->DynamicAbilityTags);
    }

    float AttackTime = GetAttackNotifyTriggerTime(MeleeAttackAbility, CombinedTags);
    if (AttackTime < 0.0f) return;

    CleanupAbilityTimers(Ability);

    TWeakObjectPtr<UAbilitySystemComponent> ASCWeak = ASC;
    TWeakObjectPtr<UGameplayAbility> AbilityWeak = Ability;
    TWeakObjectPtr<UAC_HeroEnemyAttackListener> ThisWeak = this;

    // Start timer
    FTimerDelegate TimerStartDelegate;
    TimerStartDelegate.BindLambda([ASCWeak, AbilityWeak, ThisWeak]()
        {
            if (ThisWeak.IsValid() && ASCWeak.IsValid())
            {
                // İlk window açılıyorsa tag ekle
                if (ThisWeak->ActivePerfectWindows == 0)
                {
                    ASCWeak->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect);
                }
                ThisWeak->ActivePerfectWindows++;

                if (AbilityWeak.IsValid())
                {
                    ThisWeak->ActiveAbilityTimers_Start.Remove(AbilityWeak.Get());
                }
            }
        });

    // End timer
    FTimerDelegate TimerEndDelegate;
    TimerEndDelegate.BindLambda([ASCWeak, AbilityWeak, ThisWeak]()
        {
            if (ThisWeak.IsValid() && ASCWeak.IsValid())
            {
                ThisWeak->ActivePerfectWindows--;

                // Son window kapandıysa tag'i sil
                if (ThisWeak->ActivePerfectWindows <= 0)
                {
                    ThisWeak->ActivePerfectWindows = 0; // Güvenlik için
                    ASCWeak->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect);
                }

                if (AbilityWeak.IsValid())
                {
                    ThisWeak->ActiveAbilityTimers_End.Remove(AbilityWeak.Get());
                }
            }
        });

    FTimerHandle StartHandle, EndHandle;
    GetWorld()->GetTimerManager().SetTimer(StartHandle, TimerStartDelegate, AttackTime - PerfectOffsetStart, false);
    GetWorld()->GetTimerManager().SetTimer(EndHandle, TimerEndDelegate, AttackTime + PerfectOffsetEnd, false);

    ActiveAbilityTimers_Start.Add(Ability, StartHandle);
    ActiveAbilityTimers_End.Add(Ability, EndHandle);

    Ability->OnGameplayAbilityEnded.RemoveAll(this);
    Ability->OnGameplayAbilityEnded.AddUObject(this, &UAC_HeroEnemyAttackListener::OnEnemyAbilityEnded);
}

void UAC_HeroEnemyAttackListener::OnEnemyAbilityEnded(UGameplayAbility* Ability)
{
    if (!Ability) return;

    // Timer'ların durumunu kontrol et
    bool bStartTimerExists = ActiveAbilityTimers_Start.Contains(Ability);
    bool bEndTimerExists = ActiveAbilityTimers_End.Contains(Ability);

    // Eğer Start timer yoksa ama End timer varsa = window açıktı
    bool bWindowWasActive = !bStartTimerExists && bEndTimerExists;

    // Timer'ları temizle
    CleanupAbilityTimers(Ability);

    // Eğer window açıktıysa, counter'ı düşür ve tag'i kontrol et
    if (bWindowWasActive)
    {
        ActivePerfectWindows--;
        if (ActivePerfectWindows <= 0)
        {
            ActivePerfectWindows = 0;
            if (UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(GetOwner()->FindComponentByClass<UAbilitySystemComponent>()))
            {
                ASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect);
            }
        }
    }
    // Eğer Start timer hala varsa = window henüz açılmamıştı, hiçbir şey yapma
    // Eğer her ikisi de yoksa = timer'lar zaten doğal olarak bitmişti, hiçbir şey yapma

    Ability->OnGameplayAbilityEnded.RemoveAll(this);
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
        if (const UAN_SendGameplayEvent* TagNotify = Cast<UAN_SendGameplayEvent>(Notify.Notify))
        {
            if (TagNotify->EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Attack_TraceStart)
            {
                NotifyTime = Notify.GetTriggerTime();
                break;
            }
        }
    }

    return NotifyTime >= 0.f ? NotifyTime : -1.f;
}

void UAC_HeroEnemyAttackListener::CleanupAbilityTimers(UGameplayAbility* Ability)
{
    if (!Ability || !GetWorld()) return;

    if (FTimerHandle* StartHandle = ActiveAbilityTimers_Start.Find(Ability))
    {
        GetWorld()->GetTimerManager().ClearTimer(*StartHandle);
        ActiveAbilityTimers_Start.Remove(Ability);
    }

    if (FTimerHandle* EndHandle = ActiveAbilityTimers_End.Find(Ability))
    {
        GetWorld()->GetTimerManager().ClearTimer(*EndHandle);
        ActiveAbilityTimers_End.Remove(Ability);
    }
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
        UWorld* World = GetWorld();
        auto& TM = World->GetTimerManager();

        // Start timers
        for (auto& Pair : ActiveAbilityTimers_Start)
        {
            TM.ClearTimer(Pair.Value);
        }
        ActiveAbilityTimers_Start.Empty();

        // End timers
        for (auto& Pair : ActiveAbilityTimers_End)
        {
            TM.ClearTimer(Pair.Value);
        }
        ActiveAbilityTimers_End.Empty();
    }
}