// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Tutorial/W_TutorialAbilityInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"

void UW_TutorialAbilityInfo::InitWithTutorialData(const FTutorialStepData& StepData)
{
	FTutorialAbilityData AbilityData = StepData.TutorialData;

	if (T_AbilityName)
	{
		T_AbilityName->SetText(AbilityData.AbilityName);
	}

	if (T_AbilityDescription)
	{
		T_AbilityDescription->SetText(AbilityData.AbilityDescription);
	}

	if (AbilityVisual)
	{
		if (!AbilityData.AbilityVideoMaterial.IsNull())
		{
			UMaterialInterface* LoadedMat = AbilityData.AbilityVideoMaterial.LoadSynchronous();
			if (LoadedMat)
			{
				AbilityVisual->SetBrushFromMaterial(LoadedMat);
			}
		}
	}

	TutorialStepData = StepData;
}
