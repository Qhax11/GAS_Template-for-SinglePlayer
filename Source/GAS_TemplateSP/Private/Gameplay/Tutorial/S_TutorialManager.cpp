// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Tutorial/S_TutorialManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Components/ShapeComponent.h"


void US_TutorialManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(US_SpawnDelegates::StaticClass());

    TutorialSettings = GetDefault<UDS_Tutorial>();
    if (!TutorialSettings || TutorialSettings->TutorialWidgetsByTrigger.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("TutorialSettings Settings is null in %s"), *this->GetName());
        return;
    }

    BindAllTutorailTriggers();

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnHeroSpawn.AddDynamic(this, &US_TutorialManager::OnHeroSpawn);
    }
}

void US_TutorialManager::BindAllTutorailTriggers()
{
    for (const TPair<TSoftObjectPtr<ATriggerBox>, TSoftClassPtr<UUserWidget>>& Pair : TutorialSettings->TutorialWidgetsByTrigger)
    {
        ATriggerBox* Trigger = Cast<ATriggerBox>(Pair.Key.Get());

        if (!Trigger)
        {
            UE_LOG(LogTemp, Warning, TEXT("Trigger is null or not yet loaded."));
            continue;
        }

        Trigger->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &US_TutorialManager::OnComponentOverlap);
    }
}

void US_TutorialManager::OnHeroSpawn(const FHeroSpawnData& HeroSpawnData)
{
    if (!HeroSpawnData.Character) 
    {
        return;
    }

    Hero = Cast<AGAS_HeroBase>(HeroSpawnData.Character);
    if (!Hero) 
    {
        return;
    }

    if (UAC_TargetLockSystem* HeroTargetLockSystemComp = Hero->GetTargetLockSystemComponent()) 
    {
        HeroTargetLockSystemComp->OnTargetChanged.AddDynamic(this, &US_TutorialManager::OnTargetChanged);
    }

    UAC_TagDelegates* HeroTagDelegatesComp = HeroSpawnData.Character->GetTagDelegatesComponent();
    if (!HeroTagDelegatesComp)
    { 
        return;
    }

    HeroTagDelegatesComp->RegisterDelegateForTag(
        GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnAdded).BindDynamic(this, &US_TutorialManager::OnParryKnockbackTagAdded);
    


}

void US_TutorialManager::OnParryKnockbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    UE_LOG(LogTemp, Warning, TEXT("HERO MADE KNOCKBACK!"));
}

void US_TutorialManager::OnTargetChanged(AActor* NewTarget)
{
    UE_LOG(LogTemp, Warning, TEXT("HERO TARGET LOCKED!"));
}

void US_TutorialManager::OnTutorailTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || !OtherActor->IsA<AGAS_HeroBase>() || !TutorialSettings) 
    {
        return;
    }

    ATriggerBox* Trigger = Cast<ATriggerBox>(OverlappedActor);
    if (!Trigger) 
    {
        return;
    }

    // 🔥 Key cast ile direkt Find!
    TSoftObjectPtr<AActor> TriggerKey = Trigger;

    const TSoftClassPtr<UUserWidget>* WidgetClassPtr = TutorialSettings->TutorialWidgetsByTrigger.Find(TriggerKey);
    if (WidgetClassPtr && !WidgetClassPtr->IsNull())
    {
        TSubclassOf<UUserWidget> WidgetClass = WidgetClassPtr->LoadSynchronous();
        if (WidgetClass)
        {
            UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
            if (Widget)
            {
                Widget->AddToViewport();
            }
        }
    }
}

void US_TutorialManager::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !OtherActor->IsA<AGAS_HeroBase>() || !TutorialSettings)
    {
        return;
    }

    ATriggerBox* Trigger = Cast<ATriggerBox>(OverlappedComp->GetOwner());
    if (!Trigger)
    {
        return;
    }

    // 🔥 Key cast ile direkt Find!
    TSoftObjectPtr<AActor> TriggerKey = Trigger;

    const TSoftClassPtr<UUserWidget>* WidgetClassPtr = TutorialSettings->TutorialWidgetsByTrigger.Find(TriggerKey);
    if (WidgetClassPtr && !WidgetClassPtr->IsNull())
    {
        TSubclassOf<UUserWidget> WidgetClass = WidgetClassPtr->LoadSynchronous();
        if (WidgetClass)
        {
            UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
            if (Widget)
            {
                Widget->AddToViewport();
            }
        }
    }
}


