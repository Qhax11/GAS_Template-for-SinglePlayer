// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Tutorial/W_TutorialBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"

void UW_TutorialBase::InitWithTutorialData(const FTutorialStepData& Data)
{
	if (T_AbilityName)
	{
		T_AbilityName->SetText(Data.AbilityName);
	}

	if (T_AbilityDescription)
	{
		T_AbilityDescription->SetText(Data.AbilityDescription);
	}

	if (AbilityVisual)
	{
		if (!Data.AbilityVideoMaterial.IsNull())
		{
			UMaterialInterface* LoadedMat = Data.AbilityVideoMaterial.LoadSynchronous();
			if (LoadedMat)
			{
				AbilityVisual->SetBrushFromMaterial(LoadedMat);
			}
		}
	}
}
