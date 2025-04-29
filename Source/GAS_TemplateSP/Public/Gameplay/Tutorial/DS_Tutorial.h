// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Gameplay/Tutorial/A_TutorialTrigger.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "UIManager/DS_UIManager.h"
#include "DS_Tutorial.generated.h"


USTRUCT(BlueprintType)
struct FTutorialQuestGateData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FWidgetData QuestWidgetData;

	/** Optional: The tutorial gate actor to open when this step is completed. */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class AA_TutorialGateBase> GateToOpen;
};

USTRUCT(BlueprintType)
struct FTutorialStepData
{
	GENERATED_BODY()

	/**
    * For trigger detection Gameplay Tags on actors or components.
    */
	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay.Tutorial"))
	FGameplayTag TriggerTag;

	/**
	 * The widget shown when this tutorial step starts, usually explaining the granted ability.
	 */
	UPROPERTY(EditAnywhere, Category = "UI")
	FWidgetData AbilityInfoWidgetData;

    //The quest widget class associated with this tutorial step.
	UPROPERTY(EditAnywhere, Category = "UI")
	FTutorialQuestGateData InitialQuest;

    // Leave empty if there's no chained quest.
	UPROPERTY(EditAnywhere, Category = "UI", meta = (ToolTip = "Optional: Quest widget to show after this one is completed."))
	FTutorialQuestGateData ChainedQuest;
};

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Project Systems | Subsystems | Tutorial"))
class GAS_TEMPLATESP_API UDS_Tutorial : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Tutorial")
	TArray<FTutorialStepData> TutorialSteps;

};
