// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Tutorial/DS_Tutorial.h"
#include "S_TutorialManager.generated.h"

struct FCharacterSpawnData;

UCLASS()
class GAS_TEMPLATESP_API US_TutorialManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 2. Setup
	UFUNCTION()
	void OnHeroSpawn(const FHeroSpawnData& HeroSpawnData);

	void BindAllTutorailTriggers();

	// 3. Event Handlers
	UFUNCTION()
	void OnTutorailTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnTutorialAbilityInfoClosed(const UUserWidget* ClosedTutorialWidget);

	UFUNCTION(BlueprintCallable)
	void OnQuestCompleted(const UUserWidget* CompletedQuestWidget);

	// 4. Helpers
	static void FindStepDataByQuestWidget(const UUserWidget* QuestWidget, const TArray<FTutorialStepData>& Steps, const FTutorialStepData*& OutStep, bool& bOutIsInitialQuest);
	bool IsLastQuest(const FTutorialStepData* MatchedStepData, const UUserWidget* FinishedQuestWidget);
	UUserWidget* CreateTutorialWidget(const FWidgetData WidgetData);

	// 5. Finalization
	void OnTutorialCompleted();

protected:
	UPROPERTY()
	class UUserWidget* CurrentQuestWidget = nullptr;

	class US_UIManager* UIManager;
	AGAS_HeroBase* Hero;
	UGAS_AbilitySystemComponent* HeroASC;
	APlayerController* HeroPC;
	const UDS_Tutorial* TutorialSettings;
};
