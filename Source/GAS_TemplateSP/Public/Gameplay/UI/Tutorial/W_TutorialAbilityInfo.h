// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/UI/Tutorial/W_TutorialBase.h"
#include "W_TutorialAbilityInfo.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UW_TutorialAbilityInfo : public UW_TutorialBase
{
	GENERATED_BODY()

public:
	void InitWithTutorialData(const FTutorialStepData& StepData);
	
	UPROPERTY(BlueprintReadOnly)
	FTutorialStepData TutorialStepData;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* T_AbilityName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* T_AbilityDescription;

	UPROPERTY(meta = (BindWidget))
	class UImage* AbilityVisual;
};
