// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/Listener/AC_HeroEnemyAttackListener.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Animation/AN_SendTag.h"
#include "AbilitySystemComponent.h"


UAC_HeroEnemyAttackListener::UAC_HeroEnemyAttackListener()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroEnemyAttackListener::BeginPlay()
{
	Super::BeginPlay();

    US_SpawnDelegates* SpawnSubs = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>();
    if (!SpawnSubs) return;

    // 1. GELECEK ÝÇÝN: Bundan sonra doðacaklara abone ol
    SpawnSubs->OnEnemySpawn.AddDynamic(this, &UAC_HeroEnemyAttackListener::OnEnemySpawn);

    // 2. GEÇMÝÞ ÝÇÝN: Sen yokken doðmuþ olanlarý al ve iþle
    for (const FEnemySpawnData& ExistingEnemy : SpawnSubs->AliveEnemies)
    {
        // Sanki yeni doðmuþ gibi fonksiyonu tetikle
        OnEnemySpawn(ExistingEnemy);
    }
}

void UAC_HeroEnemyAttackListener::OnEnemySpawn(const FEnemySpawnData& EnemySpawnData)
{
    if (!EnemySpawnData.Character || !EnemySpawnData.ASC)
    {
        return;
    }

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

    // Hero'nun ASC'si (Owner üzerinden alýyoruz)
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

    // Attack notify zamanýný al
    float AttackTime = GetAttackNotifyTriggerTime(MeleeAttackAbility, CombinedTags);
    if (AttackTime < 0.0f)
    {
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    float PerfectWindowStartTime = CurrentTime + (AttackTime - PerfectOffsetStart);
    float PerfectWindowEndTime = CurrentTime + (AttackTime + PerfectOffsetEnd);

    // TAG EKLE DÝREKT BÝTTÝ
    // 
    // 
    // Timer ile tag aç/kapat
    FTimerHandle TimerHandle_Start;
    FTimerHandle TimerHandle_End;

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_Start,
        FTimerDelegate::CreateLambda([ASC, this]()
            {
                ASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect);
            }),
        AttackTime - PerfectOffsetStart,
        false
    );

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_End,
        FTimerDelegate::CreateLambda([ASC, this]()
            {
                ASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect);
            }),
        AttackTime + PerfectOffsetEnd,
        false
    );
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

    if (NotifyTime < 0.f)
    {
        return -1.0f;
    }

    return NotifyTime;
}
