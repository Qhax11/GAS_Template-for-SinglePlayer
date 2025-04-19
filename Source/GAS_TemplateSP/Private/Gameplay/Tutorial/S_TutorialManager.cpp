// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Tutorial/S_TutorialManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Components/ShapeComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Gameplay/UI/Tutorial/W_TutorialBase.h"


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

  

   
    
    BindAllTutorailTriggers();


}

void US_TutorialManager::OnParryKnockbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    CurrentQuestWidget->RemoveFromParent();
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

    if (!StepData || StepData->TutorialWidgetClass.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("No tutorial step data found for tag: %s"), *TutorialTrigger->TutorialTag.ToString());
        return;
    }

    TSubclassOf<UW_TutorialBase> TutorailWidgetClass = StepData->TutorialWidgetClass.LoadSynchronous();
    if (TutorailWidgetClass)
    {
        UW_TutorialBase* TutorailWidget = CreateWidget<UW_TutorialBase>(GetWorld(), TutorailWidgetClass);
        if (TutorailWidget)
        {
            TutorailWidget->InitWithTutorialData(*StepData);
            TutorailWidget->AddToViewport();
        }
    }
}

void US_TutorialManager::OnTutorialAbilityInfoClosed(const FTutorialStepData& StepData)
{
    TSubclassOf<UUserWidget> QuestWidgetClass = StepData.QuestWidgetClass.LoadSynchronous();
    if (QuestWidgetClass)
    {
        UUserWidget* QuestWidget = CreateWidget<UUserWidget>(GetWorld(), QuestWidgetClass);
        if (QuestWidget)
        {
            QuestWidget->AddToViewport();
            CurrentQuestWidget = QuestWidget;
        }
    }

    UAC_TagDelegates* HeroTagDelegatesComp = Hero->GetTagDelegatesComponent();
    if (!HeroTagDelegatesComp)
    {
        return;
    }
    HeroTagDelegatesComp->RegisterDelegateForTag(
        GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnAdded).BindDynamic(this, &US_TutorialManager::OnParryKnockbackTagAdded);
}

