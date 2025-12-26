// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Tutorial/S_TutorialManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "LevelManager/S_LevelManager.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/UI/Tutorial/W_TutorialAbilityInfo.h"
#include "Gameplay/UI/Tutorial/W_TutorialQuest.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "UIManager/S_UIManager.h"
#include "Gameplay/Tutorial/A_TutorialGateBase.h"
#include <Kismet/GameplayStatics.h>


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

    US_SpawnDelegates* SpawnDelegatesSubsystem = GetGameInstance()->GetSubsystem<US_SpawnDelegates>();
    if (!SpawnDelegatesSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnDelegatesSubsystem is null in: %s"), *GetName());
        return;
    }

    SpawnDelegatesSubsystem->OnHeroSpawn.AddDynamic(this, &US_TutorialManager::OnHeroSpawn);
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
        UE_LOG(LogTemp, Warning, TEXT("OnTutorailTriggerBeginOverlap: No tutorial step data found for tag: %s"), *TutorialTrigger->TutorialTag.ToString());
        return;
    }

    CreateTutorialWidget(StepData->AbilityInfoWidgetData);
   
    if (TutorialTrigger->GrantedAbility.Ability)
    {
        HeroASC->GiveAbilityWithAbilityData(TutorialTrigger->GrantedAbility);
    }

    TutorialTrigger->Destroy();
}

void US_TutorialManager::OnTutorialAbilityInfoClosed(const UUserWidget* ClosedTutorialWidget)
{
    if (!ClosedTutorialWidget || !TutorialSettings)
    {
        return;
    }

    // Widget class'ını eşleştirerek ilgili StepData'yı bul
    const FTutorialStepData* StepData = TutorialSettings->TutorialSteps.FindByPredicate(
        [ClosedTutorialWidget](const FTutorialStepData& Step)
        {
            return Step.AbilityInfoWidgetData.WidgetClass
                && Step.AbilityInfoWidgetData.WidgetClass.Get() == ClosedTutorialWidget->GetClass();
        });

    if (!StepData)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnTutorialAbilityInfoClosed: Could not find StepData for ClosedTutorialWidget class: %s"), *ClosedTutorialWidget->GetClass()->GetName());
        return;
    }

    // Eğer InitialQuest'in WidgetClass'ı varsa, doğrudan WidgetData üzerinden göster
    if (!StepData->InitialQuest.QuestWidgetData.WidgetClass.IsNull())
    {
        UUserWidget* CreatedInitialQuestWidget = CreateTutorialWidget(StepData->InitialQuest.QuestWidgetData);
        if (CreatedInitialQuestWidget)
        {
            CurrentQuestWidget = CreatedInitialQuestWidget;
        }
    }
}

void US_TutorialManager::OnQuestCompleted(const UUserWidget* CompletedQuestWidget)
{
    if (!CompletedQuestWidget || !TutorialSettings)
    {
        return;
    }

    // Eğer CurrentQuestWidget bitirildiyse temizle
    if (CurrentQuestWidget == CompletedQuestWidget)
    {
        CurrentQuestWidget->RemoveFromParent();
        CurrentQuestWidget = nullptr;
    }

    const FTutorialStepData* MatchedStepData = nullptr;
    bool bIsInitialQuest = false;

    FindStepDataByQuestWidget(CompletedQuestWidget, TutorialSettings->TutorialSteps, MatchedStepData, bIsInitialQuest);

    if (!MatchedStepData)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnQuestCompleted: CompletedQuestWidget does not match any tutorial step!"));
        return;
    }

    // Doğru quest data'yı bul: Initial mi Chained mi
    const FTutorialQuestGateData& QuestData = bIsInitialQuest ? MatchedStepData->InitialQuest : MatchedStepData->ChainedQuest;

    // Eğer gate varsa aç
    if (QuestData.GateToOpen)
    {
        QuestData.GateToOpen->OpenGate();
    }

    // Eğer biten quest son quest ise tutorial tamamlandı
    if (IsLastQuest(MatchedStepData, CompletedQuestWidget))
    {
        OnTutorialCompleted();
        return;
    }

    // Eğer InitialQuest bitmişse ve ChainedQuest varsa, yeni quest widgetı spawn et
    if (bIsInitialQuest && !MatchedStepData->ChainedQuest.QuestWidgetData.WidgetClass.IsNull())
    {
        UUserWidget* CreatedChainQuestWidget = CreateTutorialWidget(MatchedStepData->ChainedQuest.QuestWidgetData);
        if (CreatedChainQuestWidget)
        {
            CurrentQuestWidget = CreatedChainQuestWidget;
        }
    }
}

void US_TutorialManager::FindStepDataByQuestWidget(const UUserWidget* QuestWidget, const TArray<FTutorialStepData>& Steps, const FTutorialStepData*& OutStep, bool& bOutIsInitialQuest)
{
    OutStep = nullptr;
    bOutIsInitialQuest = false;

    if (!QuestWidget)
    {
        return;
    }

    for (const FTutorialStepData& Step : Steps)
    {
        if (Step.ChainedQuest.QuestWidgetData.WidgetClass.IsValid() &&
            Step.ChainedQuest.QuestWidgetData.WidgetClass.Get() == QuestWidget->GetClass())
        {
            OutStep = &Step;
            bOutIsInitialQuest = false;
            return;
        }
    }

    for (const FTutorialStepData& Step : Steps)
    {
        if (Step.InitialQuest.QuestWidgetData.WidgetClass.IsValid() &&
            Step.InitialQuest.QuestWidgetData.WidgetClass.Get() == QuestWidget->GetClass())
        {
            OutStep = &Step;
            bOutIsInitialQuest = true;
            return;
        }
    }
}

bool US_TutorialManager::IsLastQuest(const FTutorialStepData* MatchedStepData, const UUserWidget* FinishedQuestWidget)
{
    if (!TutorialSettings || !MatchedStepData || !FinishedQuestWidget)
    {
        return false;
    }

    const int32 StepIndex = TutorialSettings->TutorialSteps.IndexOfByPredicate(
        [MatchedStepData](const FTutorialStepData& Step)
        {
            return &Step == MatchedStepData;
        });

    const bool bIsLastStep = (StepIndex == TutorialSettings->TutorialSteps.Num() - 1);

    const TSoftClassPtr<UUserWidget>& ChainedQuestClass = MatchedStepData->ChainedQuest.QuestWidgetData.WidgetClass;
    const bool bHasNoChainedQuest = ChainedQuestClass.IsNull();

    bool bFinishedWidgetIsChainedQuest = false;
    if (!bHasNoChainedQuest) 
    {
        if (!ChainedQuestClass.IsValid())
        {
            ChainedQuestClass.LoadSynchronous();
        }

        if (ChainedQuestClass.IsValid())
        {
            bFinishedWidgetIsChainedQuest = (ChainedQuestClass.Get() == FinishedQuestWidget->GetClass());
        }
    }

    return bIsLastStep && (bHasNoChainedQuest || bFinishedWidgetIsChainedQuest);
}

UUserWidget* US_TutorialManager::CreateTutorialWidget(const FWidgetData WidgetData)
{
    const TSoftClassPtr<UUserWidget>& WidgetClassPtr = WidgetData.WidgetClass;
    if (WidgetClassPtr.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("OnTutorailTriggerBeginOverlap: AbilityInfoWidgetData.WidgetClass is NULL in: %s"), *GetName());
        return nullptr;
    }

    UUserWidget* CreatedTutorialWidget = UIManager->CreateAndShowWidget(WidgetData);
    if (!CreatedTutorialWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateTutorialWidget: Failed to create widget in: %s"), *GetName());
        return nullptr;
    }

    return CreatedTutorialWidget;
}

void US_TutorialManager::OnTutorialCompleted()
{
    if (US_LevelManager* LevelManagerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_LevelManager>())
    {
        LevelManagerSubsystem->OpenLevel(FName("MainMenu"), false);
    }
}