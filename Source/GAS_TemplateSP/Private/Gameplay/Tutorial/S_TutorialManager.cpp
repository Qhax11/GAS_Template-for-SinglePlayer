// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Tutorial/S_TutorialManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Components/ShapeComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Gameplay/UI/Tutorial/W_TutorialAbilityInfo.h"
#include "Gameplay/UI/Tutorial/W_TutorialQuest.h"


void US_TutorialManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(US_SpawnDelegates::StaticClass());

    TutorialSettings = GetDefault<UDS_Tutorial>();
    if (!TutorialSettings || TutorialSettings->TutorialSteps.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("TutorialSettings Settings is null in %s"), *this->GetName());
        return;
    }


    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnHeroSpawn.AddDynamic(this, &US_TutorialManager::OnHeroSpawn);
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

    HeroTagDelegatesComp = Hero->GetTagDelegatesComponent();
    if (!HeroTagDelegatesComp)
    {
        return;
    }

    HeroTargetLockSystemComp = Hero->GetTargetLockSystemComponent();
    if (!HeroTargetLockSystemComp)
    {
        return;
    }
    
    BindAllTutorailTriggers();
}

void US_TutorialManager::BindAllTutorailTriggers()
{
    TArray<AActor*> FoundTriggers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AA_TutorialTrigger::StaticClass(), FoundTriggers);

    for (AActor* Actor : FoundTriggers)
    {
        AA_TutorialTrigger* TutorialTrigger = Cast<AA_TutorialTrigger>(Actor);
        if (!TutorialTrigger)
        {
            continue;
        }

        TutorialTrigger->OnActorBeginOverlap.AddDynamic(this, &US_TutorialManager::OnTutorailTriggerBeginOverlap);
    }
}

void US_TutorialManager::OnParryKnockbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    if (CurrentListenTutorialTag == GAS_Tags::TAG_Gameplay_Tutorial_Parry || CurrentQuestWidget)
    {
        CurrentQuestWidget->BP_QuestFinished();
    }
}

void US_TutorialManager::OnTargetChanged(AActor* NewTarget)
{
    if (CurrentListenTutorialTag == GAS_Tags::TAG_Gameplay_Tutorial_TargetLockSystem || CurrentQuestWidget)
    {
        CurrentQuestWidget->BP_QuestFinished();
        HeroTargetLockSystemComp->OnTargetChanged.RemoveDynamic(this, &US_TutorialManager::OnTargetChanged);
    }
}

void US_TutorialManager::OnTutorailTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || !OtherActor->IsA<AGAS_HeroBase>() || !TutorialSettings)
    {
        return;
    }

    AA_TutorialTrigger* TutorialTrigger = Cast<AA_TutorialTrigger>(OverlappedActor);
    if (!TutorialTrigger)
    {
        return;
    }

    const FTutorialStepData* StepData = TutorialSettings->TutorialSteps.FindByPredicate(
        [TutorialTrigger](const FTutorialStepData& Step)
        {
            return Step.TutorialTag == TutorialTrigger->TutorialTag;
        });

    if (!StepData || StepData->TutorialData.TutorialAbilityInfoWidgetClass.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("No tutorial step data found for tag: %s"), *TutorialTrigger->TutorialTag.ToString());
        return;
    }

    TSubclassOf<UW_TutorialAbilityInfo> TutorailAbilityInfoWidgetClass = StepData->TutorialData.TutorialAbilityInfoWidgetClass.LoadSynchronous();
    if (TutorailAbilityInfoWidgetClass)
    {
        UW_TutorialAbilityInfo* TutorailAbilityInfoWidget = CreateWidget<UW_TutorialAbilityInfo>(GetWorld(), TutorailAbilityInfoWidgetClass, FName("tut"));
        if (TutorailAbilityInfoWidget)
        {
            TutorailAbilityInfoWidget->InitWithTutorialData(*StepData);
            TutorailAbilityInfoWidget->AddToViewport();
        }
    }
}

void US_TutorialManager::OnTutorialAbilityInfoClosed(const FTutorialStepData& StepData)
{
    TSubclassOf<UW_TutorialQuest> QuestWidgetClass = StepData.QuestData.QuestWidgetClass.LoadSynchronous();
    if (QuestWidgetClass)
    {
        UW_TutorialQuest* QuestWidget = CreateWidget<UW_TutorialQuest>(GetWorld(), QuestWidgetClass);
        if (QuestWidget)
        {
            QuestWidget->AddToViewport();
            QuestWidget->InitWithTutorialData(StepData);
            CurrentQuestWidget = QuestWidget;
        }
    }

    CurrentListenTutorialTag = StepData.TutorialTag;

    // Hardcoded logic based on TutorialTag
    if (StepData.TutorialTag == GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback)
    {
        HeroTagDelegatesComp->RegisterDelegateForTag(
            GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnAdded)
            .BindDynamic(this, &US_TutorialManager::OnParryKnockbackTagAdded);
    }
    else if (StepData.TutorialTag == GAS_Tags::TAG_Gameplay_Tutorial_TargetLockSystem) // örnek başka bir tag
    {
        HeroTargetLockSystemComp->OnTargetChanged.AddDynamic(this, &US_TutorialManager::OnTargetChanged);
    }
 
   
}

