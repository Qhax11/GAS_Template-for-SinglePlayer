// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Gameplay/Tutorial/A_TutorialTrigger.h"
#include "DS_Tutorial.generated.h"


USTRUCT(BlueprintType)
struct FTutorialStepData
{
	GENERATED_BODY()

	/**
    * Tag used to identify and track this tutorial step.
    * Commonly used for trigger detection via Gameplay Tags on actors or components.
    */
	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay.Tutorial"))
	FGameplayTag TutorialTag;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<class UW_TutorialAbilityInfo> TutorialAbilityInfoWidgetClass;

    //The quest widget class associated with this tutorial step.
	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<class UW_TutorialQuest> QuestWidgetClass;
};

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Tutorial"))
class GAS_TEMPLATESP_API UDS_Tutorial : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Tutorial")
	TArray<FTutorialStepData> TutorialSteps;

};
