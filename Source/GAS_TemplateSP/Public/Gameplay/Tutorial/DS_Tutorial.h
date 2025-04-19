// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Tutorial/A_TutorialTrigger.h"
#include "DS_Tutorial.generated.h"


USTRUCT(BlueprintType)
struct FTutorialStepData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay.Tutorial"))
	struct FGameplayTag TutorialTag;

	UPROPERTY(EditAnywhere, Category = "UI")
	FText AbilityName;

	UPROPERTY(EditAnywhere, Category = "UI")
	FText AbilityDescription;

	// Video için materyal (MediaTexture kullanan dynamic material olabilir)
	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftObjectPtr<UMaterialInterface> AbilityVideoMaterial;

	// Kullanýlacak widget class (UI tipi override edilebilir)
	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<class UW_TutorialBase> TutorialWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<class UUserWidget> QuestWidgetClass;
};

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Tutorial"))
class GAS_TEMPLATESP_API UDS_Tutorial : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Tutorial")
	TArray<FTutorialStepData> TutorialSteps;

};
