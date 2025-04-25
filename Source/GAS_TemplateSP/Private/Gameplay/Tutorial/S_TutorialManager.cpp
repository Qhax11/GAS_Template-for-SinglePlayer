// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Tutorial/S_TutorialManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Components/ShapeComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Gameplay/UI/Tutorial/W_TutorialAbilityInfo.h"
#include "Gameplay/UI/Tutorial/W_TutorialQuest.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "UI/S_UIManager.h"
#include "Gameplay/Tutorial/A_TutorialGateBase.h"


void US_TutorialManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(US_SpawnDelegates::StaticClass());
    Collection.InitializeDependency(US_UIManager::StaticClass());

    TutorialSettings = GetDefault<UDS_Tutorial>();
    if (!TutorialSettings || TutorialSettings->TutorialSteps.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("TutorialSettings Settings is null in %s"), *this->GetName());
        return;
    }

    UIManager = GetGameInstance()->GetSubsystem<US_UIManager>();
    if (!UIManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager is null in: %s"), *GetName());
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

    HeroASC = Cast<UGAS_AbilitySystemComponent>(HeroSpawnData.ASC);

    HeroPC = Hero->GetPlayerController();
    if (!HeroPC)
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
            return Step.TriggerTag == TutorialTrigger->TutorialTag;
        });

    if (!StepData)
    {
        UE_LOG(LogTemp, Warning, TEXT("No tutorial step data found for tag: %s"), *TutorialTrigger->TutorialTag.ToString());
        return;
    }

    if (!StepData->TutorialAbilityInfoWidgetClass.IsNull()) 
    {
        TSubclassOf<UW_TutorialAbilityInfo> TutorailAbilityInfoWidgetClass = StepData->TutorialAbilityInfoWidgetClass.LoadSynchronous();
        UIManager->CreateAndShowWidget(TutorailAbilityInfoWidgetClass, EUIWidgetContext::Gameplay);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TutorialAbilityInfoWidgetClass is null in: %s"), *GetName());
    }

    TutorialTrigger->Destroy();
}

void US_TutorialManager::OnTutorialAbilityInfoClosed(const UW_TutorialAbilityInfo* ClosedTutorialWidget)
{
    if (!ClosedTutorialWidget || !TutorialSettings)
    {
        return;
    }

    // Widget class'ını eşleştirerek ilgili StepData'yı bul
    const FTutorialStepData* StepData = TutorialSettings->TutorialSteps.FindByPredicate(
        [ClosedTutorialWidget](const FTutorialStepData& Step)
        {
            return Step.TutorialAbilityInfoWidgetClass
                && Step.TutorialAbilityInfoWidgetClass.Get() == ClosedTutorialWidget->GetClass();
        });

    if (!StepData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not find StepData for ClosedTutorialWidget class: %s"), *ClosedTutorialWidget->GetClass()->GetName());
        return;
    }

    const TSubclassOf<UW_TutorialQuest> QuestWidgetClass = StepData->InitialQuest.QuestWidgetClass.LoadSynchronous();
    if (QuestWidgetClass)
    {
        UUserWidget* CreatedQuestWidget = UIManager->CreateAndShowWidget(QuestWidgetClass, EUIWidgetContext::Gameplay);
        if (CreatedQuestWidget) 
        {
            CurrentQuestWidget = Cast<UW_TutorialQuest>(CreatedQuestWidget);
        }
    }
}

void US_TutorialManager::OnQuestIsFinished(const UW_TutorialQuest* FinishedQuestWidget)
{
    if (!FinishedQuestWidget || !TutorialSettings)
    {
        return;
    }

    if (CurrentQuestWidget == FinishedQuestWidget)
    {
        CurrentQuestWidget->RemoveFromParent();
        CurrentQuestWidget = nullptr;
    }

    const FTutorialStepData* MatchedStepData = nullptr;
    bool bIsInitialQuest = false;

    FindTutorialStepForWidget(FinishedQuestWidget, TutorialSettings->TutorialSteps, MatchedStepData, bIsInitialQuest);

    if (!MatchedStepData)
    {
        UE_LOG(LogTemp, Warning, TEXT("FinishedQuestWidget does not match any step!"));
        return;
    }

    const FTutorialQuestGateData& QuestData = bIsInitialQuest ? MatchedStepData->InitialQuest : MatchedStepData->ChainedQuest;

    if (QuestData.GateToOpen)
    {
        QuestData.GateToOpen->OpenGate();
    }

    if (bIsInitialQuest && MatchedStepData->ChainedQuest.QuestWidgetClass.IsValid())
    {
        const TSubclassOf<UW_TutorialQuest> ChainedQuestClass = MatchedStepData->ChainedQuest.QuestWidgetClass.LoadSynchronous();
        if (ChainedQuestClass)
        {
            UUserWidget* CreatedWidget = UIManager->CreateAndShowWidget(ChainedQuestClass, EUIWidgetContext::Gameplay);
            if (CreatedWidget)
            {
                CurrentQuestWidget = Cast<UW_TutorialQuest>(CreatedWidget);
            }
        }
    }
}

void US_TutorialManager::FindTutorialStepForWidget(const UW_TutorialQuest* Widget, const TArray<FTutorialStepData>& Steps, const FTutorialStepData*& OutStep, bool& bOutIsInitialQuest)
{
    OutStep = nullptr;
    bOutIsInitialQuest = false;

    if (!Widget)
    {
        return;
    }

    // 1. Önce ChainedQuest'lere bak
    for (const FTutorialStepData& Step : Steps)
    {
        if (Step.ChainedQuest.QuestWidgetClass.LoadSynchronous() == Widget->GetClass())
        {
            OutStep = &Step;
            bOutIsInitialQuest = false;
            return;
        }
    }

    // 2. Eğer bulunamazsa InitialQuest'lere bak
    for (const FTutorialStepData& Step : Steps)
    {
        if (Step.InitialQuest.QuestWidgetClass.LoadSynchronous() == Widget->GetClass())
        {
            OutStep = &Step;
            bOutIsInitialQuest = true;
            return;
        }
    }
}

