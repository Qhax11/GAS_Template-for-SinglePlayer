// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Tutorial/W_TutorialAbilityInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"


void UW_TutorialAbilityInfo::NativeConstruct()
{
	if (T_AbilityName)
	{
		T_AbilityName->SetText(TutorialAbilityData.AbilityName);
	}

	if (T_AbilityDescription)
	{
		T_AbilityDescription->SetText(TutorialAbilityData.AbilityDescription);
	}
}

void UW_TutorialAbilityInfo::InitWithTutorialData(const FTutorialStepData& StepData)
{
	/*
	FTutorialAbilityData AbilityData = StepData.TutorialData;
	TutorialStepData = StepData;
	*/
}
