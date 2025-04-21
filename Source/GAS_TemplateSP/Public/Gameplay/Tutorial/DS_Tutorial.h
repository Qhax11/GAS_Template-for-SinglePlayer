// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Gameplay/Tutorial/A_TutorialTrigger.h"
#include "DS_Tutorial.generated.h"


USTRUCT(BlueprintType)
struct FTutorialStepData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay.Tutorial"))
	FGameplayTag TutorialTag;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<class UW_TutorialQuest> QuestWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<class UW_TutorialAbilityInfo> TutorialAbilityInfoWidgetClass;
};

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Tutorial"))
class GAS_TEMPLATESP_API UDS_Tutorial : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Tutorial")
	TArray<FTutorialStepData> TutorialSteps;

};
